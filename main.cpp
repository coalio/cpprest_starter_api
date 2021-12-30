#define _CRT_SECURE_NO_DEPRECATE
#include "modules/logging.h"
#include "modules/base/routes.h"
#include "modules/base/variables.h"

int main() {
	std::cout.sync_with_stdio(true);
	run_routes();
	return 0;
}