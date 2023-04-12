#ifndef __LOADER_DEBUGGER_H__
#define __LOADER_DEBUGGER_H__

#include <cstdint>

bool loader_is_debugger_present();
int loader_dprintf(const char* format, ...);
int loader_install_team_debugger(int debuggerTeam, int debuggerPort);
int loader_remove_team_debugger();

void loader_debugger_pre_syscall(uint32_t callIndex, void* args);
void loader_debugger_post_syscall(uint32_t callIndex, void* args, uint64_t returnValue);

#endif // __LOADER_DEBUGGER_H__