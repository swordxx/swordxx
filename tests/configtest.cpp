#include <iostream>
#include <swconfig.h>

int main(int argc, char **argv) {
    SWConfig config("./test1.conf");
    config["Section1"]["Entry1"] = "Value1";
    config["Section1"]["Entry2"] = "oops";
    config["Section1"]["Entry2"] = "Value2";
    config.Save();
    SWConfig config2("./test1.conf");
    std::cout << "Should be Value2: " << config2["Section1"]["Entry2"] << std::endl;
    return 0;
}
