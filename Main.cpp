#include "include/prot.hpp"
#include <iostream>

void foo(const std::string& text)
{
    std::cout << text << std::endl;
}

int main(void)
{
    // Direct usage of plain string (for comparison purposes).
    std::cout << "Unsafe Hello" << std::endl;

    // Test 1: Create an obfuscated string and pass it to foo.
    // This uses the default key as defined in the header file (if CUSTOM_KEY or RANDOM_KEY is not defined).
    constexpr auto text = CreateObfuscatedString("Safe Hello");
    foo(text);

    // Test 2: Obfuscate a string using the Encrypt function and output it.
    // This also uses the default key, as Encrypt uses a default key of 1.
    constexpr auto encryptedText = Encrypt("Safe Hello 2");
    std::cout << encryptedText << std::endl;

    // Test 3: Use the default obfuscation key via the Encrypt_ macro.
    // Depending on the configuration, this may use the default key or a random key.
#if DEFAULT_KEY || RANDOM_KEY
    std::cout << Encrypt_("Safe Hello 3") << std::endl;
#else
    std::cout << "DEFAULT_KEY or RANDOM_KEY is not enabled. Skipping default key test." << std::endl;
#endif

    // Test 4: Use a custom obfuscation key via the Encrypt_ macro.
    // This test should be used only if CUSTOM_KEY is enabled in the header file.
    // CUSTOM_PROT_KEY is used in this case.
#if CUSTOM_KEY
    std::cout << Encrypt_("Safe Hello 4") << std::endl;
#else
    std::cout << "CUSTOM_KEY is not enabled. Skipping custom key test." << std::endl;
#endif

    return 0;
}
