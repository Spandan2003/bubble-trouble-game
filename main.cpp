#include <simplecpp>
#include <sstream>
#include "shooter.h"
#include "power.h"

/* Simulation Vars */
const double STEP_TIME = 0.02;
const double TOTAL_TIME = 60;
const double IMMUNITY = 2;  //time for which shooter will be immune
const double FREEZE = 5;    //time for which freeze power is activated
const int FINAL_LEVEL = 10;


void display(int text, Text &disp)                          // it updates the number displayed in disp
{
    string display;
    stringstream sst;
    sst<<text;
    sst>>display;
    disp.setMessage(display);
}

void move_bullets(vector<Bullet> &bullets)                     // move all bullets
{
    for(unsigned int i=0; i<bullets.size(); i++)
    {
        if(!bullets[i].nextStep(STEP_TIME))
        {
            bullets.erase(bullets.begin()+i);
        }
    }
}

void move_bubbles(vector<Bubble> &bubbles)                      // move all bubbles
{
    for (unsigned int i=0; i < bubbles.size(); i++)
    {
        bubbles[i].nextStep(STEP_TIME);
    }
}

void move_freeze(vector<Freeze> &freeze)                      // move all freeze ball
{
    for (unsigned int i=0; i < freeze.size(); i++)
    {
        if(!freeze[i].nextStep(STEP_TIME))
        {
            freeze.erase(freeze.begin()+i);
        }
    }
}

void move_heart(vector<Heart> &heart)                      // move all heart ball
{
    for (unsigned int i=0; i < heart.size(); i++)
    {
        if(!heart[i].nextStep(STEP_TIME))
        {
            heart.erase(heart.begin()+i);
        }
    }
}

vector<Bubble> create_bubbles(int n)                            // create initial bubbles in the game
{
    vector<Bubble> bubbles;
    for(int i = 1; i<=n*2;i++)
    {
        double k = (i*i*i*i*i*i*i*i+5)%98/98.0, kvx = (1+.1*(i*i%10)); //k randomizes 0 to 1 while kvx randomizes 1 to 1.9
        int p = min(n, 3);  //Order of bubble
        bubbles.push_back(Bubble(k*WINDOW_X, BUBBLE_START_Y, BUBBLE_DEFAULT_RADIUS*pow(p, .5), -BUBBLE_DEFAULT_VX*kvx, pow(-1,i)*k*BUBBLE_DEFAULT_VY, p, COLOR(255*n*n*n%256,105*n*n%256,180*n*n*n*n%256)));
    }           //Create new bubble. here we use various terms to randomize characteristics
    return bubbles;
}

bool check_hit(vector<Bubble> &bubbles, vector<Bullet> &bullets, int &score_, Text &_score_)  // checks whether the bullet hits bubble
{
    for (unsigned int i=0; i < bubbles.size(); i++)
    {
       for (unsigned int j=0; j < bullets.size(); j++)
        {
            double dist = pow(pow(bubbles[i].get_center_y()-bullets[j].get_center_y(),2)+pow(bubbles[i].get_center_x()-bullets[j].get_center_x(),2),.5);
            //calculate distance betweem centre of bubble and bullet
            if(dist<bubbles[i].get_radius()+bullets[j].get_height()/2)
            {   //erase both bullet and bubble, increase score, If order of bubble>1 then split bubble
                score_ = score_ + 10;   //update score
                display(score_, _score_);
                bullets.erase(bullets.begin()+j);
                if(bubbles[i].get_order()>1)    //Split bubble.
                {
                    bubbles.push_back(Bubble(bubbles[i].get_center_x(), bubbles[i].get_center_y(), BUBBLE_DEFAULT_RADIUS*pow(bubbles[i].get_order()-1,.5), bubbles[i].get_vx(), bubbles[i].get_vy(), bubbles[i].get_order()-1, bubbles[i].get_color()));
                    bubbles.push_back(Bubble(bubbles[i].get_center_x(), bubbles[i].get_center_y(), BUBBLE_DEFAULT_RADIUS*pow(bubbles[i].get_order()-1,.5), -2*bubbles[i].get_vx(), bubbles[i].get_vy(), bubbles[i].get_order()-1, bubbles[i].get_color()));
                }
                bubbles.erase(bubbles.begin()+i);
                if(bubbles.size()==0)
                {
                    bullets.erase(bullets.begin(),bullets.begin()+bullets.size()); //erase all bullets on screen
                    return true;    //If there are no bubbles remaining next level.
                }
            }
        }
    }
    return false;       //There are still bubbles remaining
}

bool check_kill(vector<Bubble> &bubbles, Shooter &shooter)      //checks whether bubble hits shooter
{
    for (unsigned int i=0; i < bubbles.size(); i++)
        {
            double dist1 = pow(pow(bubbles[i].get_center_y()-shooter.get_body_center_y(),2)+pow(bubbles[i].get_center_x()-shooter.get_body_center_x(),2),.5); //calculate distance betweem centre of bubble and shooter body
            double dist2 = pow(pow(bubbles[i].get_center_y()-shooter.get_head_center_y(),2)+pow(bubbles[i].get_center_x()-shooter.get_head_center_x(),2),.5); //calculate distance betweem centre of bubble and shooter head
            if((dist1<bubbles[i].get_radius()+shooter.get_body_width()/2)||(dist2<bubbles[i].get_radius()+shooter.get_head_radius())) //bubble touches shooter
            {   //We lose
                return true;
            }
        }
    return false;
}

bool check_freeze(vector<Freeze> &freeze, Shooter &shooter)      //checks whether shooter gets freeze balls
{
    for (unsigned int i=0; i < freeze.size(); i++)
        {
            double dist1 = pow(pow(freeze[i].get_center_y()-shooter.get_body_center_y(),2)+pow(freeze[i].get_center_x()-shooter.get_body_center_x(),2),.5); //calculate distance betweem centre of freeze ball and shooter body
            double dist2 = pow(pow(freeze[i].get_center_y()-shooter.get_head_center_y(),2)+pow(freeze[i].get_center_x()-shooter.get_head_center_x(),2),.5); //calculate distance betweem centre of freeze ball and shooter head
            if((dist1<freeze[i].get_radius()+shooter.get_body_width()/2)||(dist2<freeze[i].get_radius()+shooter.get_head_radius())) //freeze ball touches shooter
            {   //We can freeze
                freeze.erase(freeze.begin()+i);
                return true;
            }
        }
    return false;
}

bool check_health(vector<Heart> &heart, Shooter &shooter)      //checks whether shooter gets health balls
{
    for (unsigned int i=0; i < heart.size(); i++)
        {
            double dist1 = pow(pow(heart[i].get_center_y()-shooter.get_body_center_y(),2)+pow(heart[i].get_center_x()-shooter.get_body_center_x(),2),.5); //calculate distance betweem centre of health ball rect and shooter body
            double dist2 = pow(pow(heart[i].get_center_y()-shooter.get_head_center_y(),2)+pow(heart[i].get_center_x()-shooter.get_head_center_x(),2),.5); //calculate distance betweem centre of health ball rect and shooter head
            double dist3 = pow(pow(heart[i].get_center_y()-heart[i].get_size()-shooter.get_body_center_y(),2)+pow(heart[i].get_center_x()-heart[i].get_size()-shooter.get_body_center_x(),2),.5); //calculate distance betweem centre of health ball left circle and shooter body
            double dist4 = pow(pow(heart[i].get_center_y()-heart[i].get_size()-shooter.get_head_center_y(),2)+pow(heart[i].get_center_x()-heart[i].get_size()-shooter.get_head_center_x(),2),.5); //calculate distance betweem centre of health ball left circle and shooter head
            double dist5 = pow(pow(heart[i].get_center_y()-heart[i].get_size()-shooter.get_body_center_y(),2)+pow(heart[i].get_center_x()+heart[i].get_size()-shooter.get_body_center_x(),2),.5); //calculate distance betweem centre of health ball right circle and shooter body
            double dist6 = pow(pow(heart[i].get_center_y()-heart[i].get_size()-shooter.get_head_center_y(),2)+pow(heart[i].get_center_x()+heart[i].get_size()-shooter.get_head_center_x(),2),.5); //calculate distance betweem centre of health ball right circle and shooter head


            if((dist1<heart[i].get_size()+shooter.get_body_width()/2)||(dist2<heart[i].get_size()+shooter.get_head_radius())||
                (dist3<heart[i].get_size()+shooter.get_body_width()/2)||(dist4<heart[i].get_size()+shooter.get_head_radius())||
                (dist5<heart[i].get_size()+shooter.get_body_width()/2)||(dist6<heart[i].get_size()+shooter.get_head_radius())) // health ball touches shooter
            {   //We can health
                heart.erase(heart.begin()+i);
                return true;
            }
        }
    return false;
}



int main()
{
    initCanvas("Bubble Trouble", WINDOW_X, WINDOW_Y);
    Line b1(0, PLAY_Y_HEIGHT, WINDOW_X, PLAY_Y_HEIGHT);
    b1.setColor(COLOR(0, 0, 255));

    string msg_cmd("Cmd: _");
    int health = 0;     //declare health, level and score
    int level;
    int score = 0;

    Text charPressed_cmd(LEFT_MARGIN, BOTTOM_MARGIN, msg_cmd);      //This block contains all text displayed in game
    Text _time(LEFT_MARGIN, TOP_MARGIN, "Timer: ");
    Text _result(WINDOW_X/2, BOTTOM_MARGIN/2, "");
    Text _health(WINDOW_X-60, TOP_MARGIN, "Health: ");
    Text _level(WINDOW_X/2, BOTTOM_MARGIN, "Level ");
    Text _score(WINDOW_X-60, BOTTOM_MARGIN, "Score: ");


    for(level = 1; level<=FINAL_LEVEL;level++)
    {
        double time = 0;    //Set time = 0 for each level
        double kill_time=0; //value of time at which shooter got hit
        double freeze_time=9999;    //value of time shooter gets freeze ball
        bool imm = true;        //Is shooter immune?
        bool iced = false;      //Is the freeze power activated?
        health = 3; //Set health as 3
        Text _level_(WINDOW_X/2+25, BOTTOM_MARGIN, level);     //This block contains all values displayed in game
        Text _time_(LEFT_MARGIN+35, TOP_MARGIN, (int)(TOTAL_TIME-time));
        Text _score_(WINDOW_X-22, BOTTOM_MARGIN, score);
        Text _health_(WINDOW_X-28, TOP_MARGIN, health);

        // Intialize the shooter
        Shooter shooter(SHOOTER_START_X, SHOOTER_START_Y, SHOOTER_VX);

        // Initialize the bubbles
        vector<Bubble> bubbles = create_bubbles(level);

        // Initialize the bullets
        vector<Bullet> bullets;

        // Initialize the freeze balls
        vector<Freeze> freeze;

        // Initialize the health balls
        vector<Heart> heart;

        XEvent event;

        // Main game loop
        while (true)
        {
            bool pendingEvent = checkEvent(event);
            beginFrame();
            if (pendingEvent)
            {
                // Get the key pressed
                char c = charFromEvent(event);
                msg_cmd[msg_cmd.length() - 1] = c;
                charPressed_cmd.setMessage(msg_cmd);

                // Update the shooter
                if(c == 'a'||c == 'A')
                    shooter.move(STEP_TIME, true);
                else if(c == 'd'||c == 'D')
                    shooter.move(STEP_TIME, false);
                else if((c == 'w'||c == 'W')&&bullets.size()<=10)
                    bullets.push_back(shooter.shoot());
                else if(c == 'q'||c == 'Q')
                    break; //return 0;
            }

            time=time+STEP_TIME;    //increase the time in each loop
            display((int)(TOTAL_TIME-time+1), _time_); //display timer
            display(health, _health_);                  // display health

            if(!iced) // Update the bubbles
                move_bubbles(bubbles);

            // Update the bullets
            move_bullets(bullets);

            // Update the freeze balls
            move_freeze(freeze);

            // Update the heart
            move_heart(heart);


            if(check_hit(bubbles, bullets, score,_score_))  //check whether bullet hits bubble
            {
                heart.erase(heart.begin(),heart.begin()+heart.size()); //erase all hearts and freeze on screen
                freeze.erase(freeze.begin(),freeze.begin()+freeze.size());
                if(level==FINAL_LEVEL)
                    _result.setMessage("You WIN");
                else
                    _result.setMessage("Next level");
                endFrame();
                wait(3);
                _result.setMessage("");
                break;
            }
            else if(time>TOTAL_TIME||health<=0)         //check whether time finished or health finished
            {
                heart.erase(heart.begin(),heart.begin()+heart.size()); //erase all hearts and freeze on screen
                freeze.erase(freeze.begin(),freeze.begin()+freeze.size());
                _result.setMessage("You LOSE");
                endFrame();
                wait(5);
                return 0;
            }

            if(time-kill_time>=IMMUNITY&&imm)       //make shooter not immune after some time after he has been hit
            {
                imm = false;
                shooter.change_color_green();
            }
            if(check_kill(bubbles, shooter)&&!imm&&!iced)     // make shooter immune if he has been hit if not already immune or bubbles not frozen. Also decrease health
            {
                kill_time=time;
                shooter.change_color_blue();
                imm = true;
                health=health-1;    //decrease health
            }
            if(time-freeze_time>=FREEZE&&iced)       //unfreeze the screen after time has passed
            {
                iced = false;
            }
            if(check_freeze(freeze, shooter))     // freeze the screen if freeze ball is picked
            {
                freeze_time=time;
                iced = true;
            }
            if(check_health(heart, shooter))     // increase health if health ball is picked
            {
                health = health + 1;
            }

            for(int i = 0; i<=TOTAL_TIME;i = i + 10)    //send a freeze ball every 10s
            {
                int t = (time+4+level); //creates a random value for x coordinate of freeze ball
                t = t%50;
                if(i-0.01<time&&time<i+0.01)
                    freeze.push_back(Freeze(t*t*t*t*t%460+20,0,10,0,200)); //Adding a freeze ball
            }

            for(int i = 5; i<=TOTAL_TIME;i = i + 20)    //send a health ball every 15s
            {
                int t = (time+level); //creates a random value for x coordinate of health ball
                t = t%50;
                if(i-0.01<time&&time<i+0.01)
                    heart.push_back(Heart(t*t*t*t%460+20,0,100)); //Adding a health ball
            }
            endFrame();
            wait(STEP_TIME);
        }
    }
}
