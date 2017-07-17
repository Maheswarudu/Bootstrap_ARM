#include "types.h"
#include "config.h"
//#include "net.h"

//#define MERS_BASE  0x40181000
//#ifdef CMD_FLASH
// #define FLASH_ADDR 0x10000000  
//#endif

#define CFG_CBSIZE              1024
#define COMMANDS

#define BOOTSTRAP_STRING "Bootstrap ARM"
#define VERSION_STRING "4.0"

#define CMD_PROMPT "Bootstrap>> "
#define MAX_ARGS 256

static char   tab_seq[] = "        ";
static char erase_seq[] = "\b \b";  

void  command_line(void);
static char *delete_char (char *, char *, int *, int *, int);
int  execute_command(char*); 
int  parse_cmd(char * , char *[]);
int  memory_display(int argc, char *argv[]);
int  memory_write(int argc, char *argv[]);
int  eeprom_getmac (int argc, char *argv[]);
int  eeprom_setmac (int argc, char *argv[]);
void display_ddr_configuration();
void write_ddr_configuration(int argc, char *argv[]);

const char bootstrap_string[] = BOOTSTRAP_STRING;
const char version_string[] = VERSION_STRING;
const char date_time_string[] = "" __DATE__ " - " __TIME__ "";

char console_buffer[CFG_CBSIZE];
char *argv[MAX_ARGS];
unsigned long load_addr;
#ifdef CMD_SPI_FLASH
  extern int spi_flash_sectors;
  extern unsigned long spi_sect_size;
#endif

#ifdef CMD_FLASH
 extern flash_info_t flash_info[];
#endif

void start_bootstrap(void)
{
   unsigned long cnf,size=0,power;
   int bootdelay = 2;

   //size = rd_d(MERS_BASE + 0x04);
   //power = rd_b(0x4026800C) & 0x7; 
   //size = 8 * (1 << power);
 
   //sys_init();
  
   PRINTF(" \n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
   PRINTF("       ---------------------------------------------------------------- \n");     
   PRINTF("                        %s     \n", bootstrap_string);
   PRINTF("       ---------------------------------------------------------------- \n\n");
   PRINTF("Build Date       : %s\n",date_time_string); 
   PRINTF("Version          : %s\n",version_string);
  //if(rd_d(0x4026800C) & ( 1 << 15))	
  // PRINTF("SDRAM size       : %d MB\n",2*size);
  //else
  // PRINTF("SDRAM size       : %d MB\n",size);
  
   #ifdef CMD_FLASH
     size = flash_init(FLASH_ADDR);
     display_flash_size(size);
   #endif
   #ifdef  CMD_SPI_FLASH
     size = get_spi_flash_size();
     display_flash_size(size);
     printf("No of Sectors    : %d\n",spi_flash_sectors); 
   #endif
   printf("\n");

#ifdef ENABLE_SPI
   if(!abortboot(bootdelay))
   {
      if (epio_spi_check_id_string())
          epio_spi_run_eeprom_cfg();
      else
         printf(" MCSTAGS not found\n");
   }
   else
#endif
   {
    #ifdef COMMANDS
      for(;;)
      {
        command_line();
        if(strlen(console_buffer)  > 1)
          execute_command(console_buffer);
      }  
    #endif 
   }

   while(1);
}

#ifdef COMMANDS

void command_line()
{
  char c;
  int  n = 0; 
  char *p = console_buffer;
  int  plen = strlen (CMD_PROMPT);  
  int  col;

  col = plen;                    

  *p = '\0';
  puts(CMD_PROMPT);

  for(;;)
  {
    //while(!tstc());
    c = getc();
    switch(c)
    {
      case '\r':	/* Enter */
      case '\n':
	     *p = '\0';
             puts ("\r\n");
	     return ;
      case 0x08:				/* ^H  - backspace	*/
      case 0x7F:				/* DEL - backspace	*/
                p=delete_char(console_buffer, p, &col, &n, plen);
		continue;
      default:
             if (n < CFG_CBSIZE-2) 
             {
                if (c == '\t') 
                { 
                   puts (tab_seq+(col&07));
                   col += 8 - (col&07);
                } 
                else 
                {
                   ++col;          /* echo input           */
                   putc (c);
                }
                *p++ = c;
                ++n;
             } 
             else                        /* Buffer full          */
                  putc ('\a');
             break;
     }
   }
}


static char * delete_char (char *buffer, char *p, int *colp, int *np, int plen)
{
        char *s;

        if (*np == 0) {
                return (p);
        }

        if (*(--p) == '\t') {                   /* will retype the whole line   */
                while (*colp > plen) {
                        puts (erase_seq);
                        (*colp)--;
                }
                for (s=buffer; s<p; ++s) {
                        if (*s == '\t') {
                                puts (tab_seq+((*colp) & 07));
                                *colp += 8 - ((*colp) & 07);
                        } else {
                                ++(*colp);
                                putc (*s);
                        }
                }
        } else {
                puts (erase_seq);
                (*colp)--;
        }
        (*np)--;
        return (p);
}


int parse_cmd (char *cmd, char *argv[])
{
   int nargs = 0;

   while (nargs < MAX_ARGS)
   {
      /* skip any white space */
      while ((*cmd == ' ') || (*cmd == '\t'))
          ++cmd;

      if (*cmd == '\0')
      {
          /* end of line, no more args  */
          argv[nargs] = NULL;
          return (nargs);
      }

      argv[nargs++] = cmd;      /* begin of argument string     */

      /* find end of string */
      while (*cmd && (*cmd != ' ') && (*cmd != '\t'))
          ++cmd;

      if (*cmd == '\0')
       {
           /* end of line, no more args */
           argv[nargs] = NULL;
           return (nargs);
       }
       *cmd++ = '\0';           /* terminate current arg         */
    }

     printf ("** Too many args (max. %d) **\n", MAX_ARGS);

     return (nargs);
}


int execute_command(char *cmd)
{
  int argc=0;
  int rc;
  int sect_first,sect_last;
  ulong src_addr,dest_addr,count;
 #ifdef CMD_FLASH
   flash_info_t *info; 
 #endif
  
 
  printf("\n");
  /* Parse the command */
  argc = parse_cmd(cmd , argv);

  /* Execute the command */
   if(strcmp(argv[0] , "help") == 0)
   {
      printf(" Supported Commands: \n\n");
      printf(" version        -  To display current version of IBOOT\n");
      printf(" reset          -  Soft reset\n");
      printf(" md             -  To display memory contents\n");
      printf(" mw             -  To write into memory\n");
      printf(" go             -  To start application at the specified address\n");
      printf(" cp             -  To copy data within RAM\n");
      printf(" eegetmac       -  To display contents of serial EEPROM\n");
      printf(" eesetmac       -  To set the MAC address\n");
    #ifdef CMD_FLASH
      printf(" erase          -  To erase flash\n");
      printf(" cp.b           -  To write data into flash\n");
    #endif
    #ifdef CMD_NET
      printf(" tftpboot       -  To boot image via network using TFTP protocol\n");
    #endif
      printf(" i2c read/write -  To copy data from RAM to I2C EEPROM or viceversa\n");
    #ifdef CMD_SPI_FLASH
      printf(" chipselect     -  To change spi flash chip select\n"); 
      printf(" spi read/write -  To copy data from RAM to SPI Flash or viceversa\n");
      printf(" spierase       -  To erase SPI flash sectors\n");
      printf(" spiconfread    -  To display spi command set in I2C\n");
      printf(" spiconfwrite   -  To write spi command set into I2C\n");  
    #endif
      printf(" ddrconfigread  -  To read the DDR configuration present in I2C\n");
      printf(" ddrconfigwrite -  To write the DDR configuration into I2C\n"); 
   }
   else if(strcmp(argv[0] , "version") == 0)
   {
    printf("iBOOT Version     : %s\n",version_string);
   }
   else if(strcmp(argv[0] , "reset") == 0)
   {
     //wr_b(0x4026C004, (rd_b(0x4026C004) & 0xFE)); /* software reset */
   }
   else if((strcmp(argv[0] , "md") == 0) || (strcmp(argv[0] , "md.b") == 0) || (strcmp(argv[0] , "md.w") == 0) || (strcmp(argv[0] , "md.l") == 0) || (strcmp(argv[0] , "md.spi") == 0) || (strcmp(argv[0] , "md.i2c") == 0))
   {
     if(argc < 2)
     {
       printf("Usage: md[.b,.w,.l,.spi,.i2c] <address> [no.of bytes]\n");
       goto end;
     }
     memory_display(argc, argv); 
   }
   else if((strcmp(argv[0] , "mw") == 0) || (strcmp(argv[0] , "mw.b") == 0) || (strcmp(argv[0] , "mw.w") == 0) || (strcmp(argv[0] , "mw.l") == 0) || (strcmp(argv[0] , "mw.spi") == 0) || (strcmp(argv[0] , "mw.i2c") == 0))
   {
     if(argc < 3)
     {
       printf("Usage: mw[.b,.w,.l,.spi,.i2c] <address> <value>  [count]\n");
       goto end;
     }
  
     memory_write(argc,argv);
   }
   else if(strcmp(argv[0] , "go") == 0)
   {
     if(argc < 2)
     {
       printf("Usage: go <address> [arg...]\n");
       goto end;
     }

     go(argc,argv);
   }
   else if((strcmp(argv[0] , "cp") == 0) || (strcmp(argv[0] , "cp.b") == 0) || (strcmp(argv[0] , "cp.w") == 0) || (strcmp(argv[0] , "cp.l") == 0))
   {
     if(argc < 4)
     {
       printf("Usage: cp[.b,.w,.l] <Source> <Destination> <count>\n");
       goto end;
     }
     memory_copy(argc,argv);
   }
   else if(strcmp(argv[0] , "eegetmac") == 0)
   {
     eeprom_getmac (argc, argv); 
   }
   else if(strcmp(argv[0] , "eesetmac") == 0)
   {
     if(argc < 2)
     {
       printf("Usage: eesetmac <MAC_addr>\n example: eesetmac 00:50:c2:1b:70:40\n\n");
       goto end;
     }
     eeprom_setmac (argc,argv);
   }
#ifdef  CMD_FLASH
    /* FLASH erase command */
   else if(strcmp(argv[0] , "erase") == 0)
   {
     if(argc !=3 )
     {
        printf("Usage: erase <start_sector>  <end_sector>\n");
        goto end;
     }
     sect_first = atoi(argv[1]);
     sect_last =  atoi(argv[2]);
 
     info = &flash_info[0];
     if((sect_first >= 0) && (sect_last >= 0) && (sect_first < CFG_MAX_FLASH_SECT) && (sect_last < CFG_MAX_FLASH_SECT))
     {
        printf ("Erase Flash from 0x%08lx to 0x%08lx... \n", info->start[sect_first], info->start[sect_last]);
        rc = flash_erase(info, sect_first, sect_last);
        if (rc == ERR_OK) 
           printf("done.\n");
     }
     else
       printf(" Bad sector specification \n");
   }
      /* FLASH write command */
   else if(strcmp(argv[0] , "cp.b") == 0)
   {
     if(argc != 4)
     {
       printf("Usage: cp.b <source_addr>  <dest_addr>  <no.of.bytes to write>\n");
       goto end;
     }
     src_addr  = convert_to_hex(argv[1]);
     dest_addr = convert_to_hex(argv[2]);
     count     = convert_to_hex(argv[3]);

     printf(" copy from 0x%08x to 0x%08x \n",src_addr,dest_addr);
     /* check if we are copying to Flash */
     if (addr2info(dest_addr) != NULL) 
     {
	printf ("Copy to Flash... ");
	rc = flash_write ((uchar *)src_addr, dest_addr, count);
    	if (rc < 0) {
      		flash_perror(rc);
                goto end;
	}
	else
      	printf("done.\n");
     } 
   }
 #endif
 #ifdef CMD_NET
   else if(strcmp(argv[0] , "tftpboot") == 0)
   {
     if(argc < 4)
     {
        printf("Usage: tftpboot <ipaddr> <serverip> <filename> <load_addr>\n");
        goto end;
     }
     load_addr = 0x200000;
     if(argc == 5)
        load_addr = convert_to_hex(argv[4]);

     if(load_addr < 0x200000) {
	printf("Please use 0x200000 and above as load address...\n");
	return 0;
	}
     ip_address = argv[1];
     serverip_address = argv[2];
     copy_filename(BootFile, argv[3], sizeof(BootFile));
      
     if (NetLoop(TFTP) == 0)
       printf(" tftp unsuccessful\n");	 
          
   }
 #endif
   else if(strcmp(argv[0] , "i2c") == 0)
   {
     if(argc != 5)
     {
       printf("Usage: i2c read/write <source_addr>  <dest_addr>  <no.of.bytes to write>\n");
       goto end;
     }
     src_addr  = convert_to_hex(argv[2]);
     dest_addr = convert_to_hex(argv[3]);
     count     = convert_to_hex(argv[4]);
     if(strcmp(argv[1] , "read") == 0)
     {
       printf(" copy from i2c offset 0x%08x to 0x%08x \n",src_addr,dest_addr);
       //i2c_eeprom_read(src_addr,dest_addr,count);

     }
     else
     {
       printf(" copy from 0x%08x to i2c offset 0x%08x \n",src_addr,dest_addr);
       //i2c_eeprom_write(src_addr,dest_addr,count);
     }

   }
 #ifdef CMD_SPI_FLASH
   else if(strcmp(argv[0] , "chipselect") == 0)
   {
     if(argc != 2)
     {
       printf("Usage: chipselect <cs num>\n");
       goto end;
     }
     set_spi_cs(convert_to_hex(argv[1]));
   }
   else if(strcmp(argv[0] , "spi") == 0)
   {
     if(argc != 5)
     {
       printf("Usage: spi read/write <source_addr>  <dest_addr>  <no.of.bytes to write>\n");
       goto end;
     }
     src_addr  = convert_to_hex(argv[2]);
     dest_addr = convert_to_hex(argv[3]);
     count     = convert_to_hex(argv[4]);
     if(strcmp(argv[1] , "read") == 0)
     {
       printf(" copy from spi offset 0x%08x to 0x%08x \n",src_addr,dest_addr);
       //spi_flash_read(src_addr,dest_addr,count);

     }
     else
     {  
       printf(" copy from 0x%08x to spi offset 0x%08x \n",src_addr,dest_addr);
       spi_flash_write(src_addr,dest_addr,count);
     }

   }
   else if(strcmp(argv[0] , "spierase") == 0)
   {
     if(argc !=3 )
     {
        printf("Usage: spierase <start_sector>  <end_sector>\n");
        goto end;
     }
     sect_first = atoi(argv[1]);
     sect_last =  atoi(argv[2]);

     if((sect_first >= 0) && (sect_last >= 0) && (sect_first < spi_flash_sectors) && (sect_last < spi_flash_sectors))
     {
        printf ("Erase Flash from 0x%08lx to 0x%08lx... \n",(sect_first * spi_sect_size) , ((sect_last * spi_sect_size)+ spi_sect_size));
        spi_flash_erase(sect_first, sect_last);
     }
     else
       printf(" Bad sector specification \n");
   }
   else if(strcmp(argv[0] , "spiconfread") == 0)
   {
      spi_command_set_display();
   }
   else if(strcmp(argv[0] , "spiconfwrite") == 0)
   {
      spi_command_set_write(argc , argv);
   }
 #endif
   else if(strcmp(argv[0] , "ddrconfigread") == 0)
   {
      display_ddr_configuration();
   }
   else if(strcmp(argv[0] , "ddrconfigwrite") == 0)
   {
      write_ddr_configuration(argc , argv);
   }
   else
      printf(" Unknown command \n");  


 end:
   printf("\n");
   return 0;
}


#define DISP_LINE_LEN	16

int memory_display(int argc, char *argv[])
{
  ulong	addr, size, length=64;
  ulong	i, nbytes, linebytes;
  uchar	*cp;
  int spi = 0;
  int i2c = 0;

  size = 4;
  
  /* Address is specified since argc > 1 */
  addr = simple_strtoul(argv[1], NULL, 16);

  /* If another parameter, it is the length to display.
   * Length is the number of objects, not number of bytes.
   */
   if (argc > 2)
   	length = simple_strtoul(argv[2], NULL, 16);

   if (argv[0][2] == '.') 
   {
       if (argv[0][3] == 'b') 
                  size = 1;
       else if (argv[0][3] == 'w') 
                  size = 2;
       else if((argv[0][3] == 's') && (argv[0][4] == 'p') && (argv[0][5] == 'i'))
       {
          size = 1;
          spi  = 1;
       }
       else if((argv[0][3] == 'i') && (argv[0][4] == '2') && (argv[0][5] == 'c'))
       {
          size = 1;
          i2c  = 1;
       }
   }


   /* Print the lines.
    *
    * We buffer all read data, so we can make sure data is read only
    * once, and all accesses are with the specified bus width.
    */
    nbytes = length * size;
    do {
		char	linebuf[DISP_LINE_LEN];
		uint	*uip = (uint   *)linebuf;
		ushort	*usp = (ushort *)linebuf;
		uchar	*ucp = (uchar *)linebuf;

		printf("%08lx:", addr);
		linebytes = (nbytes>DISP_LINE_LEN)?DISP_LINE_LEN:nbytes;
		for (i=0; i<linebytes; i+= size) {
			if (size == 4) {
				printf(" %08x", (*uip++ = *((uint *)addr)));
			} else if (size == 2) {
				printf(" %04x", (*usp++ = *((ushort *)addr)));
			} else {
                                //if(spi)
                                   //printf(" %02x", spi_read_data(addr));
                                //else if(i2c)
                                   //printf(" %02x", i2c_read_byte(addr));
                                //else
				   printf(" %02x", (*ucp++ = *((uchar *)addr)));
			}
			addr += size;
		}
		printf("    ");
		cp = linebuf;
		for (i=0; i<linebytes; i++) {
			if ((*cp < 0x20) || (*cp > 0x7e))
				printf(".");
			else
				printf("%c", *cp);
			cp++;
		}
		printf("\n");
		nbytes -= linebytes;
	} while (nbytes > 0);

   return 0;
}


int  memory_write(int argc, char *argv[])
{
	ulong	addr, size, writeval, count;
        int spi = 0;
        int i2c = 0;

	/* Check for size specification.
	*/
	size = 4;
	if (argv[0][2] == '.') 
        {
		if (argv[0][3] == 'b') 
		     size = 1;
		else if (argv[0][3] == 'w') 
		     size = 2;
		else if((argv[0][3] == 's') && (argv[0][4] == 'p') && (argv[0][5] == 'i'))
                {
                     size = 1;
                     spi = 1;
                }
		else if((argv[0][3] == 'i') && (argv[0][4] == '2') && (argv[0][5] == 'c'))
                {
                     size = 1;
                     i2c = 1;
                }
	}

	addr = simple_strtoul(argv[1], NULL, 16);

	/* Get the value to write.
	*/
	writeval = simple_strtoul(argv[2], NULL, 16);

	/* Count ? */
	if (argc == 4) {
		count = simple_strtoul(argv[3], NULL, 16);
	} else {
		count = 1;
	}

	while (count-- > 0) {
		if (size == 4)
			*((ulong  *)addr) = (ulong )writeval;
		else if (size == 2)
			*((ushort *)addr) = (ushort)writeval;
		else
                {
                     //if(spi)  
                     //   spi_write_data(addr , writeval);
                     //else if(i2c)
                     //   i2c_write_byte(addr,writeval); 
                     //else
			*((uchar *)addr) = (uchar)writeval;
                }
		addr += size;
	}

	return 0;
}

int go (int argc, char *argv[])
{
	ulong	addr, rc;
        char    *s, *e,*d,t[18];
        ulong   reg;
        unsigned char c;
        unsigned char ea[6];
        int i;


	addr = simple_strtoul(argv[1], NULL, 16);

	printf ("## Starting application at 0x%08lx ...\n", addr);

	//scp_setup_mac_addresses(bd);
         
	/*
	 * pass address parameter as argv[0] (aka command name),
	 * and all remaining args
	 */

	//cleanup_before_linux(bd); //disable interrupts, invalidate caches

	rc = ((ulong (*)(int, char *[]))addr) (--argc, &argv[1]);

	printf ("## Application terminated, rc = 0x%lx\n", rc);
	return 0;
}

int memory_copy(int argc, char *argv[])
{
   unsigned long   src_addr, size, dest_addr, count;
   unsigned char *s;

   size = 4;
   if (argv[0][2] == '.') 
   {
       if (argv[0][3] == 'b') 
           size = 1;
       else if (argv[0][3] == 'w')
           size = 2;
   }

   src_addr  = simple_strtoul(argv[1], NULL, 16);
   dest_addr = simple_strtoul(argv[2], NULL, 16);
   count     = simple_strtoul(argv[3], NULL, 16);
   
   printf(" copy from 0x%x to 0x%x \n",src_addr,dest_addr);  
   while (count-- > 0) 
   {
      if (size == 4)
          *((ulong  *)dest_addr) = *((ulong  *)src_addr);
      else if (size == 2)
          *((ushort *)dest_addr) = *((ushort *)src_addr);
      else
          *((uchar *)dest_addr) = *((uchar *)src_addr);

      src_addr += size;
      dest_addr += size;
   }

  return 0;
}


int eeprom_getmac (int argc, char *argv[])
{
  int i;
  unsigned char enetaddr[1][6];

  //i2c_get_mac_addr(enetaddr[0]); 
  for(i=0;i<6;i++)
    printf("%c%02x",i? ':':' ',enetaddr[0][i]); 
}


int eeprom_setmac (int argc, char *argv[])
{
  int i;
  char *c,*e;
  unsigned char	enetaddr[1][6];

  c = argv[1];
  for (i=0; i<6; i++) 
  {
    enetaddr[0][i] = c ? simple_strtoul(c, &e, 16) : 0;
    if (c) c = (*e) ? e+1 : e;
  }

  //i2c_set_mac_addr(enetaddr[0]);
  printf("Set MAC address to:");
  for(i=0;i<6;i++) 
     printf("%c%02x",i? ':':' ',enetaddr[0][i]);

  printf("\n");
  return 0;
}

#define DDR_CONFIG_OFFSET          0x100
#define DDR_REGISTER_CNT_OFFSET    0x104
#define DDR_REGISTER_CONFIG_OFFSET 0x110

void display_ddr_configuration()
{
 int i;
 int count;

 //count = i2c_read_byte(DDR_REGISTER_CNT_OFFSET); 

 printf(" No.of registers to be programmed %d\n",count);
 for(i = 0;i < (count*2);i=i+2)
 {
   //printf("%08x: %08x\n",i2c_read_dword(DDR_REGISTER_CONFIG_OFFSET+(i*4)),i2c_read_dword(DDR_REGISTER_CONFIG_OFFSET+((i+1)*4)));
 }
}

void write_ddr_configuration(int argc, char *argv[])
{
 int i;
 unsigned long val;

 if(argc < 4)
    printf(" Usage: ddrconfigwrite <count of registers> <16 reg values>\n");
 else
 {  
  // i2c_write_byte(DDR_CONFIG_OFFSET,0x44);   /* D */
  // i2c_write_byte(DDR_CONFIG_OFFSET+1,0x44); /* D */
  // i2c_write_byte(DDR_CONFIG_OFFSET+2,0x52); /* R */

  // i2c_write_byte(DDR_REGISTER_CNT_OFFSET,(unsigned char)simple_strtoul(argv[1], NULL, 16));
   for(i = 0;i < (argc-2) ;i++)
   {
      val = simple_strtoul(argv[i+2], NULL, 16);
    //  i2c_write_dword(DDR_REGISTER_CONFIG_OFFSET + (i*4) , val);
   } 
 }
}

#endif
