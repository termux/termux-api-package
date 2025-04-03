#include <libusb.h>

#if defined(__cplusplus)
extern "C" {
#endif

struct termux_usb_device_descriptor {
	/** USB-IF class code for the device. See \ref libusb_class_code. */
	uint8_t  bDeviceClass;

	/** USB-IF subclass code for the device, qualified by the bDeviceClass
	 * value */
	uint8_t  bDeviceSubClass;

	/** USB-IF protocol code for the device, qualified by the bDeviceClass and
	 * bDeviceSubClass values */
	uint8_t  bDeviceProtocol;

	/** USB-IF vendor ID */
	uint16_t idVendor;

	/** USB-IF product ID */
	uint16_t idProduct;

	/** String describing manufacturer */
	char     *manufacturer;

	/** String describing product */
	char     *product;

	/** String containing device serial number */
        char     *serialNumber;

	/** Number of possible configurations */
	uint8_t  bNumConfigurations;
};


struct termux_usb_device {
	uint8_t bus_number;
	uint8_t port_number;
	char *device_address;

	struct termux_usb_device_descriptor *device_descriptor;
};

struct termux_usb_endpoint_descriptor {
	/** The address of the endpoint described by this descriptor. Bits 0:3 are
	 * the endpoint number. Bits 4:6 are reserved. Bit 7 indicates direction,
	 * see \ref libusb_endpoint_direction. */
	uint8_t  bEndpointAddress;

	/** Attributes which apply to the endpoint when it is configured using
	 * the bConfigurationValue. Bits 0:1 determine the transfer type and
	 * correspond to \ref libusb_endpoint_transfer_type. Bits 2:3 are only used
	 * for isochronous endpoints and correspond to \ref libusb_iso_sync_type.
	 * Bits 4:5 are also only used for isochronous endpoints and correspond to
	 * \ref libusb_iso_usage_type. Bits 6:7 are reserved. */
	uint8_t  bmAttributes;

	/** Maximum packet size this endpoint is capable of sending/receiving. */
	uint16_t wMaxPacketSize;

	/** Interval for polling endpoint for data transfers. */
	uint8_t  bInterval;
};

struct termux_usb_interface_descriptor {
	/** Number of this interface */
	uint8_t  bInterfaceNumber;

	/** Value used to select this alternate setting for this interface */
	uint8_t  bAlternateSetting;

	/** Number of endpoints used by this interface (excluding the control
	 * endpoint). */
	uint8_t  bNumEndpoints;

	/** USB-IF class code for this interface. See \ref libusb_class_code. */
	uint8_t  bInterfaceClass;

	/** USB-IF subclass code for this interface, qualified by the
	 * bInterfaceClass value */
	uint8_t  bInterfaceSubClass;

	/** USB-IF protocol code for this interface, qualified by the
	 * bInterfaceClass and bInterfaceSubClass values */
	uint8_t  bInterfaceProtocol;

	/** String describing this interface */
	char     *interface;

	/** Array of endpoint descriptors. This length of this array is determined
	 * by the bNumEndpoints field. */
        struct termux_usb_endpoint_descriptor *endpoint;
};

struct termux_usb_config_descriptor {
	/** Number of interfaces supported by this configuration */
	uint8_t  bNumInterfaces;

	/** Identifier value for this configuration */
	uint8_t  bConfigurationValue;

	/** String describing this configuration */
	char     *configuration;

	/** Maximum power consumption of the USB device from this bus in this
	 * configuration when the device is fully operation. Expressed in units
	 * of 2 mA when the device is operating in high-speed mode and in units
	 * of 8 mA when the device is operating in super-speed mode. */
	uint8_t  MaxPower;

	/** Array of interfaces supported by this configuration. The length of
	 * this array is determined by the bNumInterfaces field. */
	struct termux_usb_interface_descriptor *interface;
};

ssize_t termux_usb_get_device_list(struct termux_usb_device ***);
void termux_usb_free_device_list(struct termux_usb_device **);
intptr_t termux_usb_open_address(char *);
intptr_t termux_usb_open(struct termux_usb_device *);
int termux_usb_get_config_descriptor(struct termux_usb_device *, int,
				     struct termux_usb_config_descriptor **);
void termux_usb_free_config_descriptor(struct termux_usb_config_descriptor *);
#if defined(__cplusplus)
}
#endif
