
/* WARNING: Unknown calling convention */

int zaflagread(char *arg)

{
  FILE *__stream;
  int iVar1;
  FILE *flf;
  
  builtin_strncpy(za_flag_31337,"ThisIsNotTheRealFlag{31337}",0x1c);
  __stream = fopen("flag","r");
  if (__stream == (FILE *)0x0) {
    iVar1 = -1;
  }
  else {
    iVar1 = strcmp(arg,"darnutellas66");
    if (iVar1 == 0) {
      fgets(za_flag_31337,100,__stream);
      fclose(__stream);
      iVar1 = 0;
    }
    else {
      fclose(__stream);
      iVar1 = -2;
    }
  }
  return iVar1;
}


/* WARNING: Unknown calling convention */

void send_success(char *result)

{
  size_t sVar1;
  int iVar2;
  packet *ppVar3;
  packet succ_pkt;
  
  ppVar3 = &succ_pkt;
  for (iVar2 = 0x42; iVar2 != 0; iVar2 = iVar2 + -1) {
    ppVar3->len = 0;
    ppVar3 = (packet *)&ppVar3->code;
  }
  succ_pkt.code = 0x7f00;
  if (result != (char *)0x0) {
    strncpy(succ_pkt.data,result,0x68);
    sVar1 = strlen(succ_pkt.data);
    succ_pkt.len = sVar1 + 1;
  }
  send_packet(state.fd,&succ_pkt);
  return;
}


/* WARNING: Unknown calling convention */

int send_packet(int fd,packet *pkt)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int res;
  int total;
  
  iVar1 = send_bytes(fd,(char *)pkt,4);
  iVar3 = iVar1;
  if ((-1 < iVar1) && (iVar2 = send_bytes(fd,(char *)&pkt->code,2), iVar3 = iVar2, -1 < iVar2)) {
    iVar3 = send_bytes(fd,pkt->data,pkt->len);
    if (-1 < iVar3) {
      iVar3 = iVar1 + iVar2 + iVar3;
    }
  }
  return iVar3;
}


/* WARNING: Unknown calling convention */

int main(int argc,char **argv)

{
  int iVar1;
  sockaddr local_1a0;
  sockaddr local_190;
  socklen_t local_180;
  packet local_17c;
  char local_74 [80];
  int local_24;
  int local_20;
  int local_1c;
  int local_18;
  int local_14;
  int *local_c;
  
  local_c = &argc;
  local_14 = 0;
  local_18 = 0;
  if ((1 < argc) && (local_14 = strtol(argv[1],(char **)0x0,10), local_14 == 0)) {
    return 1;
  }
  state.logfunc = default_log;
  local_1c = -1;
  local_20 = -1;
  if (local_14 < 1) {
    state.fd = 0;
    iVar1 = state.fd;
  }
  else {
    local_1c = socket(2,1,0);
    if (local_1c < 0) {
      return 1;
    }
    local_190.sa_family = 2;
    local_190.sa_data._0_2_ = htons((uint16_t)local_14);
    local_190.sa_data._2_4_ = inet_addr("127.0.0.1");
    iVar1 = bind(local_1c,&local_190,0x10);
    if (iVar1 < 0) {
      return 1;
    }
    iVar1 = listen(local_1c,1);
    if (iVar1 < 0) {
      return 1;
    }
    local_180 = 0x10;
    local_20 = accept(local_1c,&local_1a0,&local_180);
    iVar1 = local_20;
    if (local_20 < 0) {
      return 1;
    }
  }
  state.fd = iVar1;
  zaflagread("darnutellas66");
LAB_08049c94:
  while( true ) {
    if (local_18 != 0) {
      if (-1 < local_1c) {
        close(local_20);
        close(local_1c);
      }
      fflush(stdout);
      sleep(1);
      return 0;
    }
    local_24 = recv_packet(state.fd,&local_17c);
    if (local_24 < 0) {
      send_error("packet receive failed!");
    }
    snprintf(local_74,0x50,"received pkt code=%i, len=%i!",(int)local_17c.code,local_17c.len);
    if (state.logfunc != (logfunc_t)0x0) {
      (*state.logfunc)(local_74);
    }
    if (local_17c.len < 0x101) break;
    send_error("packet too large!");
  }
  if (local_17c.code == 0x7999) {
                    /* WARNING: Subroutine does not return */
    exit(0);
  }
  if (local_17c.code < 0x799a) {
    if (local_17c.code == 0x2200) {
      handle_subscribe(local_17c.data);
      goto LAB_08049c94;
    }
    if (0x2200 < local_17c.code) goto LAB_08049c61;
    if (local_17c.code == 0x1000) {
      handle_add_topic(local_17c.data);
      goto LAB_08049c94;
    }
    if (local_17c.code == 0x2100) {
      handle_publish(local_17c.data);
      goto LAB_08049c94;
    }
  }
LAB_08049c61:
  snprintf(local_74,0x50,"invalid packet code: %i!",(int)local_17c.code);
  send_error(local_74);
  local_18 = 1;
  goto LAB_08049c94;
}


/* WARNING: Unknown calling convention */

void handle_subscribe(char *data)

{
  int iVar1;
  char tmp [100];
  char *topic;
  int i;
  
  i = 0;
  while( true ) {
    if (8 < i) {
      snprintf(tmp,100,"Topic \'%s\' not found!",data);
      send_error(tmp);
      return;
    }
    iVar1 = strcmp(state.topics[i].name,data);
    if (iVar1 == 0) break;
    i = i + 1;
  }
  send_success(state.topics[i].last_message);
  return;
}


/* WARNING: Unknown calling convention */

int handle_add_topic(char *name)

{
  int iVar1;
  topic *__dest;
  int iVar2;
  packet *ppVar3;
  packet reply;
  int idx;
  
  iVar1 = state.topics_count;
  ppVar3 = &reply;
  for (iVar2 = 0x42; iVar2 != 0; iVar2 = iVar2 + -1) {
    ppVar3->len = 0;
    ppVar3 = (packet *)&ppVar3->code;
  }
  if (state.topics_count < 9) {
    __dest = state.topics + state.topics_count;
    state.topics_count = state.topics_count + 1;
    strcpy(__dest->name,name);
    send_success("Topic has been added!");
  }
  else {
    send_error("No more topics allowed!");
    iVar1 = -1;
  }
  return iVar1;
}

