# termux-api-package

This package contains userspace tools to call exposed API methods in
the [Termux:API](https://github.com/termux/termux-api) app.

The core functionality is put in a libtermux-api.so library, and can
be accessed either by linking directly to it, or by calling the
termux-api-broadcast program, which basically just forwards given args
to the `run_api_command` function in libtermux-api.so.  (bash) scripts
for using the various API methods are available in the scripts/
directory.  These make use of termux-api-broadcast.

