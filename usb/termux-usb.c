#include <libusb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "termux-api.h"
#include "termux-usb.h"
#include "UsbAPI.pb-c.h"

#define BUF_SIZE 1024
#define TERMUX_PREFIX "/data/data/com.termux/files/usr"

static size_t read_buffer (unsigned int buf_size, uint8_t *out, FILE *fp)
{
	size_t cur_len = 0;
	size_t nread;
	while ((nread = fread(out + cur_len, 1, buf_size - cur_len, fp)) != 0)
	{
		cur_len += nread;
		if (cur_len >= buf_size)
		{
			fprintf(stderr, "max message length exceeded\n");
			exit(EXIT_FAILURE);
		}
	}
	return cur_len;
}

ssize_t termux_usb_get_device_list(struct termux_usb_device ***list)
{
	Usbapi__TermuxUsb *container;
	struct termux_usb_device **ret;
	uint8_t buf[BUF_SIZE];
	FILE *fp;
	unsigned int num_devices;

	fp = popen(TERMUX_PREFIX "/libexec/termux-api Usb -a getDevices", "r");
	if (fp == NULL) {
		perror("popen");
		exit(EXIT_FAILURE);
	}

	size_t msg_len = read_buffer(BUF_SIZE, buf, fp);
	if (pclose(fp)) {
		fprintf(stderr, "Error closing protobuf stream\n");
		return -1;
	}

	container = usbapi__termux_usb__unpack(NULL, msg_len, buf);
	if (container == NULL)
	{
		fprintf(stderr, "Error unpacking incoming message\n");
		return -1;
	}

	ret = (struct termux_usb_device **) calloc(container->n_device + 1, sizeof(struct termux_usb_device *));
	if (!ret) {
		fprintf(stderr, "Failed to allocate memory\n");
		usbapi__termux_usb__free_unpacked(container, NULL);
		return -1;
	}

	ret[container->n_device] = NULL;
	for (size_t i = 0; i < container->n_device; i++) {
		Usbapi__TermuxUsbDevice *msg = container->device[i];
		Usbapi__TermuxUsbDeviceDescriptor *desc = msg->device;
		struct termux_usb_device *dev = (struct termux_usb_device *) malloc(sizeof(struct termux_usb_device));
		struct termux_usb_device_descriptor *dev_desc = (struct termux_usb_device_descriptor *) malloc(sizeof(struct termux_usb_device_descriptor));

		dev->bus_number = msg->busnumber;
		dev->port_number = msg->portnumber;

		dev->device_address = (char *) malloc(strlen(msg->deviceaddress)+1);
		strcpy(dev->device_address, msg->deviceaddress);

		dev_desc->bDeviceClass = desc->deviceclass;
		dev_desc->bDeviceSubClass = desc->devicesubclass;
		dev_desc->bDeviceProtocol = desc->deviceprotocol;
		dev_desc->idVendor = desc->vendorid;
		dev_desc->idProduct = desc->productid;

		dev_desc->manufacturer = (char *) malloc(strlen(desc->manufacturername)+1);
		strcpy(dev_desc->manufacturer, desc->manufacturername);

		dev_desc->product = (char *) malloc(strlen(desc->productname)+1);
		strcpy(dev_desc->product, desc->productname);

		dev_desc->serialNumber = (char *) malloc(strlen(desc->serialnumber)+1);
		strcpy(dev_desc->serialNumber, desc->serialnumber);

		dev_desc->bNumConfigurations = desc->configurationcount;

		dev->device_descriptor = dev_desc;
		ret[i] = dev;
	}
	*list = ret;

	num_devices = container->n_device;
	usbapi__termux_usb__free_unpacked(container, NULL);
	return num_devices;
}

void termux_usb_free_device_list(struct termux_usb_device **list)
{
	int i = 0;
	struct termux_usb_device *dev;
	while ((dev = list[i++]) != NULL) {
		free(dev->device_descriptor->manufacturer);
		free(dev->device_descriptor->product);
		free(dev->device_descriptor->serialNumber);
		free(dev->device_descriptor);
		free(dev->device_address);
		free(dev);
	}
	free(list);
}

intptr_t termux_usb_open_address(char *address)
{
	int argc = 10;
	char *argv[argc];
	argv[0] = "termux-usb";
	argv[1] = "Usb";
	argv[2] = "-a";
	argv[3] = "open";
	argv[4] = "--ez";
	argv[5] = "request";
	argv[6] = "true";
	argv[7] = "--es";
	argv[8] = "device";
	argv[9] = address;

	int fd = run_api_command(argc, argv);

	return (intptr_t) fd;
}

intptr_t termux_usb_open(struct termux_usb_device *dev)
{
	return termux_usb_open_address(dev->device_address);
}

int termux_usb_get_config_descriptor(struct termux_usb_device *dev, int config_index,
				     struct termux_usb_config_descriptor **conf_desc)
{
	Usbapi__TermuxUsbConfigDescriptor *confDesc;
	uint8_t buf[BUF_SIZE];
	FILE *fp;

	char cmd[256];
	char *base_cmd = TERMUX_PREFIX "/libexec/termux-api Usb -a getConfigDescriptor --es device %s --ei config %d";
	snprintf(cmd, sizeof(cmd)-1, base_cmd, dev->device_address, config_index);
	fp = popen(cmd, "r");
	if (fp == NULL) {
		perror("popen");
		return 1;
	}

	size_t msg_len = read_buffer(BUF_SIZE, buf, fp);
	if (pclose(fp)) {
		fprintf(stderr, "Error closing protobuf stream\n");
		return -1;
	}

	confDesc = usbapi__termux_usb_config_descriptor__unpack(NULL, msg_len, buf);
	if (confDesc == NULL) {
		fprintf(stderr, "Error unpacking incoming config descriptor message\n");
		return -1;
	}

	struct termux_usb_config_descriptor *conf;
        conf = (struct termux_usb_config_descriptor *) malloc(sizeof(struct termux_usb_config_descriptor));
	if (conf == NULL) {
		fprintf(stderr, "Memory allocation failed\n");
		usbapi__termux_usb_config_descriptor__free_unpacked(confDesc, NULL);
		return -1;
	}

	conf->bConfigurationValue = confDesc->configurationvalue;
	conf->configuration = (char *) malloc(strlen(confDesc->configuration)+1);
	if (conf->configuration == NULL) {
		fprintf(stderr, "Memory allocation failed\n");
		free(conf);
		usbapi__termux_usb_config_descriptor__free_unpacked(confDesc, NULL);
		return -1;
	}
	strcpy(conf->configuration, confDesc->configuration);
	conf->MaxPower = confDesc->maxpower;
	conf->bNumInterfaces = confDesc->n_interface;

	conf->interface = (struct termux_usb_interface_descriptor *) calloc(confDesc->n_interface, sizeof(struct termux_usb_interface_descriptor));
	if (conf->interface == NULL) {
		fprintf(stderr, "Memory allocation failed\n");
		free(conf->configuration);
		free(conf);
		usbapi__termux_usb_config_descriptor__free_unpacked(confDesc, NULL);
		return -1;
	}

	for (size_t i = 0; i < confDesc->n_interface; i++) {
		Usbapi__TermuxUsbInterfaceDescriptor *intfDesc = confDesc->interface[i];

		conf->interface[i].bAlternateSetting = intfDesc->alternatesetting;
		conf->interface[i].bNumEndpoints = intfDesc->n_endpoint;
		conf->interface[i].bInterfaceClass = intfDesc->interfaceclass;
		conf->interface[i].bInterfaceSubClass = intfDesc->interfacesubclass;
		conf->interface[i].bInterfaceProtocol = intfDesc->interfaceprotocol;
		conf->interface[i].interface = (char *) malloc(strlen(intfDesc->interface)+1);
		if (conf->interface[i].interface == NULL) {
			fprintf(stderr, "Memory allocation failed\n");
			for (size_t j = 0; j < i; i++) {
				free(conf->interface[i].endpoint);
				free(conf->interface[i].interface);
			}
			free(conf->interface);
			free(conf->configuration);
			free(conf);
			usbapi__termux_usb_config_descriptor__free_unpacked(confDesc, NULL);
			return -1;
		}
		strcpy(conf->interface[i].interface, intfDesc->interface);

		conf->interface[i].endpoint = (struct termux_usb_endpoint_descriptor *) calloc(intfDesc->n_endpoint, sizeof(struct termux_usb_endpoint_descriptor));
		if (conf->interface[i].endpoint == NULL) {
			fprintf(stderr, "Memory allocation failed\n");
			for (size_t j = 0; j < i; i++) {
				free(conf->interface[i].endpoint);
				free(conf->interface[i].interface);
			}
			free(conf->interface);
			free(conf->configuration);
			free(conf);
			usbapi__termux_usb_config_descriptor__free_unpacked(confDesc, NULL);
		}

		for (size_t j = 0; j < intfDesc->n_endpoint; j++) {
			Usbapi__TermuxUsbEndpointDescriptor *endpointDesc = intfDesc->endpoint[j];
			conf->interface[i].endpoint[j].bEndpointAddress = endpointDesc->endpointaddress;
			conf->interface[i].endpoint[j].bmAttributes = endpointDesc->attributes;
			conf->interface[i].endpoint[j].wMaxPacketSize = endpointDesc->maxpacketsize;
			conf->interface[i].endpoint[j].bInterval = endpointDesc->interval;
		}
	}
	*conf_desc = conf;

	usbapi__termux_usb_config_descriptor__free_unpacked(confDesc, NULL);
	return 0;
}

void termux_usb_free_config_descriptor(struct termux_usb_config_descriptor *conf_desc)
{
	for (int i = 0; i < conf_desc->bNumInterfaces; i++) {
		free(conf_desc->interface[i].endpoint);
		free(conf_desc->interface[i].interface);
	}
	free(conf_desc->interface);
	free(conf_desc->configuration);
	free(conf_desc);
}
