// Serial code include file
int read_usb_serial_none_blocking(char *buf,int length);

int write_usb_serial_blocking(char *buf,int length);

void serial_init(void);

void debug_to_serial(const char* format , ...);

void clear_serial(void);
