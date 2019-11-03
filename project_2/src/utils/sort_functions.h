#include <string>
#include "info/process_info.h"

using namespace std;

bool pidSort(const ProcessInfo& a, const ProcessInfo& b);
bool cpuSort(const ProcessInfo& a, const ProcessInfo& b);
bool memSort(const ProcessInfo& a, const ProcessInfo& b);
bool timeSort(const ProcessInfo& a, const ProcessInfo& b);

void sortProcesses(vector<ProcessInfo>& processes, string sortby);
