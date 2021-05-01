#include "header.hpp"
 
int main() {
    network_func();

    FILE* pFile = fopen("settings.json", "rb");
    char buffer[65536];
    FileReadStream is(pFile, buffer, sizeof(buffer));
    Document document;
    document.ParseStream<0, UTF8<>, FileReadStream>(is);

    assert(document["username"].IsString());
    printf("username = %s\n", document["username"].GetString());

    // SFML_rendering();

    return 0;
}