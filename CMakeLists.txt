cmake_minimum_required(VERSION 3.0.0)
project(termux-api)
include(GNUInstallDirs)

add_executable(termux-api termux-api.c)

set(TERMUX_PREFIX ${CMAKE_INSTALL_PREFIX})

# TODO: get list through regex or similar
set(script_files
  scripts/termux-audio-info
  scripts/termux-battery-status
  scripts/termux-brightness
  scripts/termux-call-log
  scripts/termux-camera-info
  scripts/termux-camera-photo
  scripts/termux-clipboard-get
  scripts/termux-clipboard-set
  scripts/termux-contact-list
  scripts/termux-dialog
  scripts/termux-download
  scripts/termux-fingerprint
  scripts/termux-infrared-frequencies
  scripts/termux-infrared-transmit
  scripts/termux-job-scheduler
  scripts/termux-keystore
  scripts/termux-location
  scripts/termux-media-player
  scripts/termux-media-scan
  scripts/termux-microphone-record
  scripts/termux-nfc
  scripts/termux-notification
  scripts/termux-notification-list
  scripts/termux-notification-remove
  scripts/termux-sensor
  scripts/termux-share
  scripts/termux-sms-inbox
  scripts/termux-sms-list
  scripts/termux-sms-send
  scripts/termux-speech-to-text
  scripts/termux-storage-get
  scripts/termux-telephony-call
  scripts/termux-telephony-cellinfo
  scripts/termux-telephony-deviceinfo
  scripts/termux-toast
  scripts/termux-torch
  scripts/termux-tts-engines
  scripts/termux-tts-speak
  scripts/termux-usb
  scripts/termux-vibrate
  scripts/termux-volume
  scripts/termux-wallpaper
  scripts/termux-wifi-connectioninfo
  scripts/termux-wifi-enable
  scripts/termux-wifi-scaninfo
)

make_directory(scripts)
foreach(file ${script_files})
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/${file}.in
    ${file} @ONLY
  )
endforeach()

install(
  FILES ${CMAKE_BINARY_DIR}/termux-api
  DESTINATION ${CMAKE_INSTALL_PREFIX}/libexec
  PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
    GROUP_READ GROUP_EXECUTE
    WORLD_READ WORLD_EXECUTE
)

foreach(file ${script_files})
  install(
    FILES ${CMAKE_BINARY_DIR}/${file}
    TYPE BIN
    PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
  )
endforeach()
