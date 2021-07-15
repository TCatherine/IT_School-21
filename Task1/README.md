# Task1 - Downloader
>https://ru.wikipedia.org/wiki/CURL
>
>https://curl.se/libcurl/c/libcurl-easy.html
>
>Напишите программу на C++, которая использует libcurl easy interface, чтобы скачивать файл из сети по заданному url. Например, >https://protei.ru/themes/custom/aga/favicon.ico.
>
>Тут основной упор на подключение и использование сторонней библиотеке на C, а наш язык разработки C++. Надо изучить API и самостоятельно написать RAII обертку над >библиотечными вызовами libcurl.
>
>https://ru.wikipedia.org/wiki/Получение_ресурса_есть_инициализация
>
>Не пишите в стиле C, для владения ресурсом CURL используйте std::unique_ptr с переопределенным deleter'ом.
>
>std::unique_ptr<CURL, decltype(&curl_easy_cleanup)> curl{curl_easy_init(), &curl_easy_cleanup};
>Напишите класс, который содержит объект типа std::unique_ptr<CURL, decltype(&curl_easy_cleanup)> с методом для скачивания файла по URL.
>
>В дальнейшем эта наработка будет использоваться в других заданиях, например, в задании по MNP.
>
