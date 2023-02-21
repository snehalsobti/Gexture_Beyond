from __future__ import print_function, unicode_literals


from search import search
from player import play_song
from pyfiglet import Figlet
import serial
import time
import random


EXIT_TOGGLE = False


def main():
    ser = serial.Serial('COM5', 38400)
    time.sleep(2)
    data = []  # empty list to store the data
    done = 0
    query = ""

    while done == 0:
        b = ser.readline()  # read a byte string
        string_n = b.decode()  # decode byte string into Unicode
        string = string_n.rstrip()  # remove \n and \r
        if string == "Happy":
            query = "Dance music"
            done = 1
        elif string == "Stressed":
            query = "Soothing music"
            done = 1
        print(string)
        time.sleep(0.1)  # wait (sleep) 0.1 seconds


    #query = prompt(questions, style=command_style)

    search_results = search(query)

    music_to_play = random.randint(0, 4)

    #choice = list_search_results(search_results)

    play_song(search_results[music_to_play]['value'])

    ser.close()
    #questions = [
    #   {
    #        'type': 'input',
    #        'name': 'query',
    #        'message': 'Search:',
    #    }
    #]


"""
def list_search_results(search_list):
    questions = [
        {
            'type': 'list',
            'name': 'search',
            'message': 'Search Results:',
            'choices': search_list,
        },
    ]

    answer = prompt(questions, style=command_style)

    return answer


def prompt_to_continue():
    questions = [
        {
            'type': 'confirm',
            'message': 'Do you want to continue?',
            'name': 'continue',
            'default': True,
        },
    ]

    answer = prompt(questions, style=command_style)

    return not answer['continue']
"""

if __name__ == '__main__':
    # Big Header
    f = Figlet(font='slant')
    print(f.renderText('Gexture Beyond'))

    # Main Life Cycle Loop
    while True:

        if EXIT_TOGGLE:
            break

        EXIT_TOGGLE = main()

    print("Thanks for using!!")
