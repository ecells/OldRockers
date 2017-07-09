# OldRockers
Arduino Challenge 2017

This project aims at creating an intelligent watering system.

It is based on Arduino and on Telegram Bot for the remote control.

Telegram Bot is controlled with https://github.com/reo7sp/tgbot-cpp

I downloaded the Docker base image from https://hub.docker.com/r/reo7sp/tgbot-cpp/. Follow the instructions to build the library at https://github.com/reo7sp/tgbot-cpp.

The syntax to start the docker container is: docker run -v /Users/mapl/Documents/GitHub/OldRockers/TelegramBotLibrary:/home/OldRockers/TelegramBotLibrary/tgbot-cpp --rm -it reo7sp/tgbot-cpp
(You will have to replace "/Users/mapl/Documents/GitHub/OldRockers/TelegramBotLibrary" with the local directory of the OldRockers github project)

The code to interact with the Telegram Bot and send messages to the Arduino is here (once you start the Docker): home/OldRockers/TelegramBotLibrary/tgbot-cpp/tgbot-cpp/samples/main.cpp

You will have to connect the laptop to your Arduino MKR1000, open the terminal, read the IP address assigned to it and modify the IP address in main.cpp: #define IPArd <IP address>

The Telegram Bot is called "ControlRiego_bot". It is started with "/start".
