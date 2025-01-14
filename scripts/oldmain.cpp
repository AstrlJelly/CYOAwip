#include <iostream>
#include "util.h"

using namespace std;

string honorific;

void wrongAnswer()
{
    cout << "haha. you put the wrong answer in. restart timeee ^_^";
}

int main()
{
    honorific = readLine("What would you like your honorific to be? I.e 'sir', 'sire', 'miss', 'missus' : ");
    // honorific = tolower(honorific);
    string option1 = readLine("You are in a hotel lobby. You can either take the elevator to your 'room', or try to strike up a conversation with the mysterious 'stranger' sitting on the couch. : ");
    if (option1 == "room") {
        room:
        string option2 = readLine("You go to your room. The drive here was tough on you! All you want to do is relax. Will you go onto the 'balcony' or will you pour yourself a 'drink'? : ");
        if (option2 == "balcony") {
            readLine("As you step onto the balcony, absorbing the pleasant orange hue of the sunset...");
            readLine("In your happiest and yet weakest moment, a bird twice your size swoops down and takes you to feed her younglings.");
            readLine("How unfortunate... Oh, well. At least you died for a good cause.");
        } else if (option2 == "drink") {
            readLine("Ahhh... that goes down easy.");
            readLine("The orange juice they stock here is some primo stuff.");
            readLine("You feel yourself drifting off, knowing tomorrow will be even better than today.");
        } else {
            wrongAnswer();
        }
    } else if (option1 == "stranger") {
        readLine("You greet the stranger. She has long, jet black hair,\nand looks to be tall. You swear you saw sharp teeth, as well..?");
        readLine("\"Hellooo, younggg oneee.\" What a charmer.\n\"Youuu don't look a dayyyy under 52oooo!\" Oh.");
        string option2 = readLine("\"Iiii am known as a merrrchant arounnnd these partsss..\nwould you like to seeee my waresss?\" ('yes', 'no') : ");
        if (option2 == "yes") {
            readLine("\"Ffffffantastic. Not mmanyyy exxxercise curiousssityyy with my waresss.\"\nYou get the feeling they're fairly justified in that.");
            string option3 = readLine("\"Purrrchase... purrrchase...\"\nYou see one bottle of half empty 'soda', a used 'bandage',\na bootlegged 'cd', and a ziploc bag of... insect 'eggs'?\nMaybe it's best to just 'not' buy anything... : ");
            if (option3 == "not" || option3 == "no") {
                readLine("\"Youuuuuu... youuuuu darrrrrre? Youuuuu dareee nnnot take my waressssss?\"");
                readLine("Uh oh.");
                readLine("She opens her mouth. Seems they were justified, after all.");
                readLine("Those teeth sure look sharp! Unfortunately, it's too late to get away from her now.");
            }
        } else if (option2 == "no") {
            readLine("You decide against it.\nMysterious wares, sold by a mysterious woman? That's a formula for a mysterious night.");
            readLine("\"Ohh... alrighttt... You haveee a good nighttt, " + honorific + "...\"");
            goto room;
        } else {
            wrongAnswer();
        }
    } else {
        wrongAnswer();
    }
    
    return 0;
}