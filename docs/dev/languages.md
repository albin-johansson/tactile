# Languages

This document explains how internationalization is implemented in Tactile, and how to add new languages.

## Overview

In order to support multiple languages, strings are not hardcoded in the application.
Instead, all user-facing strings are loaded at application startup, and stored in structs.
Use the `get_current_language` function to obtain a reference to the set of strings for the currently selected language.

It's not necessary to translate all strings when adding a new language (see the `en_GB.json` file for an example), the language parser will use a fallback if a translation is missing.
When adding a new language in the code, use English as the fallback language.

```C++
void some_widget()
{
  // BAD
  if (Menu menu {"Foo"}; menu.is_open()) {
    // ...
  }

  // GOOD
  const auto& lang = get_current_language();
  if (Menu menu {lang.menu.foo.c_str()}; menu.is_open()) {
    // ...
  }
}
```

## Adding a new language

The process of adding a new supported language in the code is the easy part.
The basic procedure consist of the following steps.

* Add a JSON file with the translated strings to `assets/lang/`
  * The filename should at least be the ISO 639-1 code for the language, e.g. `sv.json` for Swedish. Translations for a specific region or country, such as British English, use the format `en_GB.json`.
* Update the `Lang` enum, and make sure the new language is selectable in the settings dialog
* Update the `Lang` enum in the settings protobuf file
* Make the `load_languages` function load the new language file
* Add a case for the new language in `get_language_name`

## Adding a new translated string

* At the very least, add the new string to the `en.json` file.
  * The name of the entry in the JSON file should mirror the name of the associated string member, but use `kebab-case` in JSON and `snake_case` for the string member
* Add an appropriately named `string` member to one of the string collection structs, e.g. `ActionStrings`
* Update the language parser to load the newly added string
