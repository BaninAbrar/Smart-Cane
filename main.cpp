    /**
    *Smart Cane
    *Programmers: Salmaan Khan, Banin Abrar, Iram Rahman
    *
    *This program will run on Omega 2 and it will run multiple commands when certain buttons are pressed
    *such as turn on LEDs, display date and time, and work as a stopwatch
    */

    //all of our #includes
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <unistd.h>
    #include <fcntl.h>
    #include <iostream>
    #include <ctime>
    #include <cstdlib>
    #include <ugpio/ugpio.h> //Omega2 gpio lib
    #include <time.h>       /* time_t, struct tm, time, localtime */
    #include "distance.c" //pulsein() for distance calculation

    using namespace std;

    //defines constants
    #define HIGH 1
    #define LOW 0

    //function for printing out the current date
    char* rip(const struct tm *timeptr)
    {
      //const char array that stores the day of the week
      static const char wday_name[][10] =
      {
        "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"
      };

      //const char array that stores the months of the year
      static const char mon_name[][10] =
      {
        "January", "February", "March", "April", "May", "June",
        "July", "August", "September", "October", "November", "December"
      };

      //print current date
      
      static char result[26];
      sprintf(result, "%.10s %.8s%3d %d\n",
        wday_name[timeptr->tm_wday],
        mon_name[timeptr->tm_mon],
        timeptr->tm_mday,
        1900 + timeptr->tm_year);
      return result;
    }

    //function for printing out the current time
    
    char* timee(const struct tm *timeptr)
    {
      static char result[26];
      sprintf(result, "%.2d:%.2d\n",
        (timeptr->tm_hour)%12,
        timeptr->tm_min);
      return result;
    }


    int main()
    {

      //Requesting all pins


      gpio_request (0,NULL);

      gpio_request (1,NULL);

      gpio_request (2,NULL);

      gpio_request (7,NULL);

      gpio_request (3,NULL);

      gpio_request (9,NULL);


     //Setting direction for the pins

      
      gpio_direction_input(0);
      gpio_direction_input(1);
      gpio_direction_input(7);
      gpio_direction_input(3);
      gpio_direction_input(9);

    //setting one pin as the output
      
      gpio_direction_output(2,0);

    //declares integer variables that will store the values read in by all the pins
    
    int gpio0;
    int gpio1;
    int gpio7;
    int gpio3;
    int gpio9;

    //declares and initializes the boolean variables that are used to check if buttons have been pressed
    
    bool temp = false;
    bool temp2 = false;
    bool temp3 = false;
    bool temp4 = false;

    //declares timer_t object used to for the stopwatch
    time_t timer;
    struct tm y2k = {0};

    //declares double variables used for calculating elapsed time for the stopwatch
    double seconds;
    double seconds2;
    double difference;

    //declares and initializes counter
    int counter = 0;

    //sets y2k time
    y2k.tm_hour = 0;   y2k.tm_min = 0; y2k.tm_sec = 0;
    y2k.tm_year = 100; y2k.tm_mon = 0; y2k.tm_mday = 1;

    //declares variables for getting current time
    time_t rawtime;
    struct tm * timeinfo;
    time (&rawtime);
    timeinfo = localtime (&rawtime);


    while (true)
    {

      //reading value from all input pins

      gpio0=gpio_get_value(0);

      gpio1=gpio_get_value(1);

      gpio7=gpio_get_value(7);

      gpio3=gpio_get_value(3);

      gpio9=gpio_get_value(9);

      /*if input on pin 0 is active
      *(button on pin 0 is pressed)
      *This is the turn on LED function
      */
      if (gpio0)
      {
        //turns on LEDs
        gpio_set_value(2, HIGH);
        //sets boolean temp variable to be true
        temp = true;
      }

      /*if input on pin 1 is active
      *(button on pin 1 is pressed)
      *This is the blinking LED function
      */
      else if (gpio1)
      {
        //turns on LEDs
        gpio_set_value(2,HIGH);

        //delay 250ms
        usleep (250000);

        //turns off LEDs
        gpio_set_value(2,LOW);

        //delay 250ms
        usleep (250000);

      }

      /*if input on pin 7 is active
      *(button on pin 7 is pressed)
      */
      else if (gpio7 && !temp2)
      {
        //gets current date and outputs it to ssh
        printf ("The current date is: %s", rip(timeinfo));

        //sets boolean temp2 variable to true
        temp2 = true;
      }

      /*if input on pin 3 is active
      *(button on pin 3 is pressed)
      *This is the current time function
      */
      else if (gpio3 && !temp3)
      {
        //gets current time and outputs it to ssh
        printf ("The current time is: %s", timee(timeinfo));

        //sets boolean temp3 variable to true
        temp3 = true;
      }

      /*if input on pin 9 is active
      *(button on pin 9 is pressed)
      *this is the stopwatch function
      */
      else if(gpio9 && !temp4)
      {

        //gets current time
        time(&timer);

        //if the counter is odd (the button has been pressed twice, meaning the stopwatch is stopped)
        if(counter % 2)
        {
          //gets current time since Jan. 1, 2000
          seconds2 = difftime(timer,mktime(&y2k));

          //gets the time that has elapsed since the button was first pressed
          difference = seconds2 - seconds;

          //output the difference
          cout << "The time that has passed is: " << difference << " seconds" << endl;
        }
        else
        {
          //output to tell user that the stopwatch has begun
          cout << "The stopwatch has started. Press again to stop." << endl;

          //gets current time since Jan 1, 2000
          seconds = difftime(timer,mktime(&y2k));
        }

        //increments counter
        counter++;

        //sets temp4 boolean variable to true
        temp4 = true;
      }


      /**checks if the user has let go of the button on pin 0
      *also checks if the temp variable has been turned to true
      *this signifies the button on pin 0 was pressed and let go
      */
      if(!gpio0 && temp)
      {
        //turns off the LEDs
        gpio_set_value(2,LOW);

        //sets the temp variable to false
        temp = false;
      }


        /**checks if the user has let go of the button on pin 7
        *also checks if the temp2 variable has been turned to true
        *this signifies the button on pin 7 was pressed and let go
        */
      if (!gpio7 && temp2)
      {

        //sets temp2 variable to false
        temp2=false;
      }


        /**checks if the user has let go of the button on pin 3
        *also checks if the temp3 variable has been turned to true
        *this signifies the button on pin 3 was pressed and let go
        */
      if (!gpio3 && temp3)
      {

        //sets temp3 variable to false
        temp3 = false;
      }


        /**checks if the user has let go of the button on pin 9
        *also checks if the temp4 variable has been turned to true
        *this signifies the button on pin 9 was pressed and let go
        */
      if (!gpio9 && temp4)
      {

        //sets temp4 variable to false
        temp4 = false;
      }

    }

    //frees the Pins
    
    gpio_free(0);  
    gpio_free(1);
    gpio_free(2);
    gpio_free(3);
    gpio_free(7);
    gpio_free(9);

    return 0;
    }
