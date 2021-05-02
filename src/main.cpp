#include "header.hpp"

int main() {
    struct Settings settings_struct;

    json_parser_create(&settings_struct);
    SFML_GUI(settings_struct);

    // network_func(settings_struct);

    return 0;
}