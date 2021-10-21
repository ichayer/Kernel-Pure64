#include <stdint.h>
#include <naiveConsole.h>
#include <graphicMode.h>
#include <systemCalls.h>
#include <keyboard.h>
#include <time.h>
#include <RTClock.h>

color_t STD = {0xFF,0xFF,0xFF};
color_t BG = {0x00,0x00,0x00};
color_t ERR = {0xFF,0x00,0x00};

int sys_write(uint64_t fd, char * buffer, uint64_t size, uint64_t screen_id) {
  if (buffer == 0 || size == 0 || fd > 2)
      return -1;

  color_t col = ((fd == STDERR) ? ERR : STD);

  uint64_t i = 0;
  while(i < size && buffer[i])
    printCharFormatId(screen_id,buffer[i++],&col, &BG);
  return i;
}

int sys_read(uint64_t fd, char * buffer, uint64_t size) {
  if (buffer == 0 || size == 0 || fd != 0)
      return -1;

    uint8_t i = 0;
    int c;

    while( i<size  &&  ((c = getChar()) != -1)  ){
      buffer[i++] = c;
    }
    return i;
}

void sys_date(char * buffer){
  get_date(buffer);
}

void sys_time(char * buffer){
  get_time(buffer);
}

int sys_hasTicked(){
    static unsigned long last_tick = 0;
    unsigned long current_tick = ticks_elapsed();
    if(last_tick == current_tick)
        return 0;
    last_tick = current_tick;
    return 1;
}

void sys_clearWindow(uint8_t id){
   clearAll(id);
}

void sys_restartCursor(uint8_t id){
  restartCursor(id);
}

void sys_divide(){
  initDividedWindow();
}

void sys_uniqueWindow(){
  initUniqueWindow();
}

int sysCallDispatcher(uint64_t fd, uint64_t buffer, uint64_t size, uint64_t screen_id, uint64_t syscall_id) {
  switch(syscall_id){
      case 0:
        return sys_read(fd,buffer,size);

      case 1:
        return sys_write(fd,buffer,size,screen_id);

      case 2:
        get_time(fd);
        return 0;

      case 3:
         return sys_hasTicked();

      case 4:
        sys_clearWindow(fd);
        return 0;

      case 5:
        sys_restartCursor(fd);
        return 0;

      case 6:
        sys_divide();
        return 0;

      case 7:
        sys_uniqueWindow();
        return 0;

  }
  return -1;
}
