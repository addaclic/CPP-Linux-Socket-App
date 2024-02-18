# TestQuestRITAutomation
It is necessary to write two applications in C++: client, server. These applications must communicate via the TCP/IP protocol.

## Тестовое задание
Необходимо написать на языке С++ два приложения: клиент, сервер.
Данные приложения должны взаимодействовать через протокол TCP/IP.
### Клиент
Консольное приложение, которое из аргументов командной строки берет
IP и порт сервера, производит подключение к серверу. В случае успешного
подключения выводится соответствующее сообщение и команда ожидает
ввода команд для отправки на сервер. В случае ошибки приложение
выводит сообщение и завершает работу. В случае потери соединения
приложение должно пытаться переподключиться.
### Сервер
Консольное приложение, которое позволяет подключать несколько
клиентов одновременно и производит обработку команд, поступающих от
них.
#### Функционал сервера
- Парсинг сообщения от клиента и возврат сообщения, содержащего
табличку с количеством различных букв:
```
Message Hello
H       1
e       1
l       2
o       1
```
- Возврат количества текущих подключений по запросу клиента
- Возможность общения клиентов через сервер
### Требования
- Программы должны отрабатывать корректно без зависаний и
вылетаний
- Структура проекта должна быть хорошо сформирована
- При разработке приложения придерживаться принципов ООП
- Должен быть реализован требуемый функционал
- Программа должна быть написана без использования фреймворков,
только используя стандартные средства языков C/C++ и ОС Linux
- ПО должно быть написано для работы под ОС Linux
- Для сборки ПО должен использоваться либо cmake, либо make (cmake
предпочтительнее)