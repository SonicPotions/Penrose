/* Intel HEX read functions, Paul Stoffregen, paul@ece.orst.edu */
/* This code is in the public domain.  Please retain my name and */
/* email address in distributed copies, and let me know about any bugs */

/* I, Paul Stoffregen, give no warranty, expressed or implied for */
/* this software and/or documentation provided, including, without */
/* limitation, warranty of merchantability and fitness for a */
/* particular purpose. */

/* ported to C++ class by julian schmidt 2014
*/


#include <stdio.h>
#include <string.h>


class Hex2Bin
{
public:
  Hex2Bin()
  {
  }
  ~Hex2Bin()
  {
  }
  
  int getSize() 
  {
      return maxaddr;
  }
  
  int* getData()
  {
      return memory;
  }
  
  
  
  /* this loads an intel hex file into the memory[] array */
  /* loads an intel hex file into the global memory[] array */
  /* filename is a string of the file to be opened */
  void load_file(char *filename)
  {
	  char line[1000];
	  FILE *fin;
	  int addr, n, status, bytes[256];
	  int i, total=0, lineno=1;
	  minaddr=65536, maxaddr=0;

	  if (strlen(filename) == 0) {
		  printf("   Can't load a file without the filename.");
		  printf("  '?' for help\n");
		  return;
	  }
	  fin = fopen(filename, "r");
	  if (fin == NULL) {
		  printf("   Can't open file '%s' for reading.\n", filename);
		  return;
	  }
	  while (!feof(fin) && !ferror(fin)) {
		  line[0] = '\0';
		  fgets(line, 1000, fin);
		  if (line[strlen(line)-1] == '\n') line[strlen(line)-1] = '\0';
		  if (line[strlen(line)-1] == '\r') line[strlen(line)-1] = '\0';
		  if (parse_hex_line(line, bytes, &addr, &n, &status)) {
			  if (status == 0) {  /* data */
				  for(i=0; i<=(n-1); i++) {
					  memory[addr] = bytes[i] & 255;
					  total++;
					  if (addr < minaddr) minaddr = addr;
					  if (addr > maxaddr) maxaddr = addr;
					  addr++;
				  }
			  }
			  if (status == 1) {  /* end of file */
				  fclose(fin);
				  printf("   Loaded %d bytes between:", total);
				  printf(" %04X to %04X from hex file\n", minaddr, maxaddr);
				  return;
			  }
			  if (status == 2){} ;  /* begin of file */
		  } else {
			  printf("   Error: '%s', line: %d\n", filename, lineno);
		  }
		  lineno++;
	  }
  }
private:
  /* this is used by load_file to get each line of intex hex */
  /* parses a line of intel hex code, stores the data in bytes[] */
/* and the beginning address in addr, and returns a 1 if the */
/* line was valid, or a 0 if an error occured.  The variable */
/* num gets the number of bytes that were stored into bytes[] */
  int parse_hex_line(char *theline, int bytes[], int *addr, int *num, int *code)
  {
	int sum, len, cksum;
	char *ptr;
	
	*num = 0;
	if (theline[0] != ':') return 0;
	if (strlen(theline) < 11) return 0;
	ptr = theline+1;
	if (!sscanf(ptr, "%02x", &len)) return 0;
	ptr += 2;
	if ( strlen(theline) < (11 + (len * 2)) ) return 0;
	if (!sscanf(ptr, "%04x", addr)) return 0;
	ptr += 4;
	  /* printf("Line: length=%d Addr=%d\n", len, *addr); */
	if (!sscanf(ptr, "%02x", code)) return 0;
	ptr += 2;
	sum = (len & 255) + ((*addr >> 8) & 255) + (*addr & 255) + (*code & 255);
	while(*num != len) {
		if (!sscanf(ptr, "%02x", &bytes[*num])) return 0;
		ptr += 2;
		sum += bytes[*num] & 255;
		(*num)++;
		if (*num >= 256) return 0;
	}
	if (!sscanf(ptr, "%02x", &cksum)) return 0;
	if ( ((sum & 255) + (cksum & 255)) & 255 ) return 0; /* checksum error */
	return 1;
  }
  
  
  int	memory[65536];		/* the memory is global */
  int minaddr, maxaddr;
};












