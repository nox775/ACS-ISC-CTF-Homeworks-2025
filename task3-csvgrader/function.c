
/* WARNING: Unknown calling convention */

int main(int argc,char **argv)

{
  int iVar1;
  char *pcVar2;
  double local_430;
  char local_428 [1028];
  undefined4 local_24;
  FILE *local_20;
  int local_1c;
  double local_18;
  int *local_10;
  
  local_10 = &argc;
  local_18 = 0.0;
  local_1c = 0;
  local_20 = stdin;
  if (1 < argc) {
    iVar1 = strcmp(argv[1],"-");
    if (iVar1 != 0) {
      local_20 = fopen(argv[1],"r");
    }
  }
  if (local_20 == (FILE *)0x0) {
    perror("Error opening csv file");
    iVar1 = 2;
  }
  else {
    local_24 = 0;
    while( true ) {
      pcVar2 = fgets(local_428,0x404,local_20);
      if (pcVar2 == (char *)0x0) break;
      iVar1 = parse_csv_line(local_428,&local_430);
      if (iVar1 < 1) {
        printf("WARNING: parsing line #%i failed!\n",local_24);
      }
      else {
        local_18 = local_18 + local_430;
        local_1c = local_1c + 1;
      }
    }
    fclose(local_20);
    printf("Total sum of grades: %.2f\n",local_18._0_4_,(int)((ulonglong)local_18 >> 0x20));
    if (local_1c < 1) {
      puts("No valid grades found in the file.");
    }
    else {
      printf("Average grade: %.2f\n",local_18 / (double)local_1c);
    }
    iVar1 = 0;
  }
  return iVar1;
}

