extern void serial_init();
extern void start_bootstrap(void);

void sys_init(void)
{
	serial_init();
	start_bootstrap();
}
