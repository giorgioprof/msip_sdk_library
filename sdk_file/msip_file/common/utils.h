#include <stdlib.h>
#include "mip/common_types.h"
#include "mip_cc/application_info_cc.h"

using std::cout;
using std::endl;
using mip::ApplicationInfo;

int ValidateOptionsforAppInfo(const mip_cc_application_info& appInfo) {
  // Application options
  if (!appInfo.applicationId) {
    cout << "ERROR: No Application Id found" << endl;
    return EXIT_FAILURE;
  }
  if (!appInfo.applicationName) {
    cout << "ERROR: No Application name found" << endl;
    return EXIT_FAILURE;
  }
  if (!appInfo.applicationVersion) {
    cout << "ERROR: No Application version found" << endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int ValidateOptionsforAppInfo(const ApplicationInfo& appInfo) {
  // Application options
  if (appInfo.applicationId.empty()) {
    cout << "ERROR: No Application Id found" << endl;
    return EXIT_FAILURE;
  }
  if (appInfo.applicationName.empty()) {
    cout << "ERROR: No Application name found" << endl;
    return EXIT_FAILURE;
  }
  if (appInfo.applicationVersion.empty()) {
    cout << "ERROR: No Application version found" << endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

