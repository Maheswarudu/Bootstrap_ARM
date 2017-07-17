extern void serial_init();
extern void start_bootstrap(void);

void sys_init(void)
{
	serial_init();
	printf("Hello Bootstrap\n");

	printf("bootstrap>\n");
	//while(1)
	//{
	//	printf("bootstrap>\n");
	//}
	start_bootstrap();
}
