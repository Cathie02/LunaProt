#ifndef LUNA_PROT_H
#define LUNA_PROT_H

#include <utility>
#include <string>

/*           ,.-----__
          ,:::://///,:::-.
         /:''/////// ``:::`;/|/
        /'   ||||||     :://'`\
      .' ,   ||||||     `/(  x \
-===~__-'\__X_`````\_____/~`-._ `.
            ~~        ~~       `~-'
C++ Compile-time string obfuscation for C++14 and later made by LunaLynx.

This header provides a mechanism for obfuscating and encrypting strings at compile time
to help protect sensitive text data in your applications. The obfuscation is done using
simple key-based transformations that can be customized at compile time.
*/

#pragma region CONSTANTS
    #define CUSTOM_PROT_KEY        0x5
#pragma endregion CONSTANTS

# pragma region CONFIG
    #define DEFAULT_KEY            1
    #define CUSTOM_KEY             0
    #define RANDOM_KEY             0
# pragma endregion CONFIG

#if (DEFAULT_KEY + CUSTOM_KEY + RANDOM_KEY) > 1
    #error "Only one config can be enabled at a time"
#endif

#if (CUSTOM_KEY == 1) && (CUSTOM_PROT_KEY == 0)
    #error "CUSTOM_PROT_KEY must be defined and not 0"
#endif

template<typename T, size_t Size>
class ObfuscatedString
{
public:
    // Constructor with obfuscation key
    constexpr ObfuscatedString(const T(&data)[Size], T key = 1) :
        m_Key(key), m_Data{}
    {
        for (size_t i = 0; i < Size - 1; ++i)
        {
            m_Data[i] = Obfuscate(data[i]);
        }
    }

    // Constructor for dynamic strings (size not known at compile time)
    explicit ObfuscatedString(const T* data, size_t size, T key = 1) :
        m_Key(key), m_Data{}
    {
        for (size_t i = 0; i < size; ++i)
        {
            m_Data[i] = Obfuscate(data[i]);
        }
    }

    // Conversion operator to std::string or std::wstring
    operator std::basic_string<T>() const
    {
        T result[Size] = { 0 };
        for (size_t i = 0; i < Size - 1; ++i)
        {
            result[i] = m_Data[i] - m_Key;
        }
        return std::basic_string<T>(result);
    }

    // Comparison operators
    bool operator==(const ObfuscatedString& other) const
    {
        return std::memcmp(m_Data, other.m_Data, Size * sizeof(T)) == 0;
    }

    bool operator!=(const ObfuscatedString& other) const
    {
        return !(*this == other);
    }

    bool operator!=(const std::basic_string<T>& str) const
    {
        return !(*this == str);
    }

private:
    constexpr T Obfuscate(const T& element) const
    {
        return element + m_Key;
    }

private:
    T m_Key;                // Obfuscation key
    T m_Data[Size] = { 0 }; // Obfuscated data
};

// Output stream operator
template<typename T, size_t Size>
std::ostream& operator<<(std::ostream& os, const ObfuscatedString<T, Size>& obfuscatedString)
{
    return os << static_cast<std::basic_string<T>>(obfuscatedString);
}

// Create obfuscated string from character array
template<typename T, size_t Size>
constexpr auto CreateObfuscatedString(const T(&data)[Size], T key = 1) -> ObfuscatedString<T, Size>
{
    return ObfuscatedString<T, Size>(data, key);
}

// Encryption function for char and wchar_t
template<size_t Size>
constexpr auto Encrypt(const char(&data)[Size], char key = 1) -> ObfuscatedString<char, Size>
{
    return CreateObfuscatedString(data, key);
}

template<size_t Size>
constexpr auto Encrypt(const wchar_t(&data)[Size], wchar_t key = 1) -> ObfuscatedString<wchar_t, Size>
{
    return CreateObfuscatedString(data, key);
}

#if CUSTOM_KEY
#define Encrypt_(str) ([]() { \
    static constexpr auto encrypted = Encrypt(str, CUSTOM_PROT_KEY); \
    return encrypted; \
}())
#elif RANDOM_KEY
#define Encrypt_(str) ([]() { \
	static constexpr auto encrypted = Encrypt(str, __TIME__[7] - '0'); \
    return encrypted; \
}())
#else // DEFAULT_KEY
#define Encrypt_(str) ([]() { \
    static constexpr auto encrypted = Encrypt(str); \
    return encrypted; \
}())
#endif

#endif
