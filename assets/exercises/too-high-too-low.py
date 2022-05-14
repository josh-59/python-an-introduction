
import random

num = random.randint(0,1000)

print("I've got my number!")

usr_input = int(input("What is your guess? "))

while usr_input != num:
    if usr_input < num:
        print("Too low!")
    else:
        print("Too high!")
    usr_input = int(input("What is your guess? "))

print("Correct!")
