#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <cmath>


void set_random_seed();
int randn(int n);
bool permutation = false;

struct mm_code_maker {

        void init(int i_length, int i_num){
                length = i_length;
                num = i_num;
        }

        void generate_sequence(){
                for(int i = 0; i < length; i++) {
                        sequence.push_back(randn(num));
                }
        }

        void give_feedback(const std::vector<int>& attempt, int& black_hits, int& white_hits){
                black_hits = 0;
                white_hits = 0;
                int totalhits = 0;
                std::vector<int>tempsequence;
                tempsequence = sequence;
                for(int i = 0; i < length; i++) {
                        if(sequence[i] == attempt[i]) {
                                black_hits++;
                        }
                }

                for(int i = 0; i < tempsequence.size(); i++) {
                        bool hasalreadybeenhit = false;
                        for(int j = 0; j < tempsequence.size(); j++) {
                                if((tempsequence[j]== attempt[i])&&(hasalreadybeenhit == false)) {
                                        tempsequence[j] = -1;
                                        totalhits++;
                                        hasalreadybeenhit = true;
                                }

                        }
                }
                white_hits = totalhits - black_hits;
        }

        std::vector<int> sequence;
        int length;
        int num;
};




struct mm_solver {

        bool firstattempt;
        std::vector<std::vector<int> > possiblepermutations;
        std::vector<std::vector<int> > tempset;
        std::vector<std::vector<int> > allattempts;
        std::vector<int> permutatedcode;
        std::vector<int> prevvector;
        int relativeblack;
        int relativewhite;
        int length;
        int num;
        bool elimination;
        bool permutation;
        bool increment;
        int numofattempts;
        int black_hits_total;
        bool possible;
        int prevbh;
        int prevwh;
        int elementindex;
        int elementvalue;
        std::vector<int> tempattempt;
        std::vector<int> whitehitelements;




        void init(int i_length, int i_num){
                length = i_length;
                num = i_num;
                firstattempt = true;
                int maxcomb = power(num,length);
                if(maxcomb > 10000000) {
                        increment = true;
                        elimination = false;
                }
                else{
                        elimination = true;
                        increment = false;
                }
                permutation = falsexx;


                numofattempts = 0;
                black_hits_total = 0;
                possible = false;
                prevbh = 0;
                prevwh = 0;
                elementindex = 0;
                elementvalue = 0;


        }

        int power(int n, int p) {
                int end = 1;
                for (int i = 0; i < p; ++i) {
                        end = end * n;
                }
                return end;
        }


        void create_attempt(std::vector<int>& attempt){
                if(elimination == true) {
                        if(firstattempt == true) {
                                attempt.push_back(0);
                                for (int i = 0; i < length-1; i++) {
                                  if(i >= num){
                                    attempt.push_back(num-1);
                                  }
                                  else{
                                        attempt.push_back(i);
                                      }
                                }
                                allattempts.push_back(attempt);
                        }
                        else{

                                int randomchoice = std::rand() % possiblepermutations.size();
                                attempt = possiblepermutations[randomchoice];
                                for(int i = 0; i < allattempts.size(); i++) {
                                        if(attempt == allattempts[i]) {
                                                possiblepermutations[std::rand() % possiblepermutations.size()];
                                                i = 0;
                                        }
                                }
                                allattempts.push_back(attempt);
                        }
                }
                if(permutation == true) {
                        if(black_hits_total < length) {
                                if(numofattempts < num) {
                                        for(int i = 0; i < length; i++) {
                                                attempt.push_back(numofattempts);
                                        }
                                        numofattempts++;
                                }
                        }
                        else{
                                if((black_hits_total == length)&&(firstattempt == true)) {
                                        attempt = permutatedcode;
                                        allattempts.push_back(attempt);
                                        numofattempts++;
                                        firstattempt = false;
                                }
                                else{
                                        int randomchoice = std::rand() % possiblepermutations.size();
                                        attempt = possiblepermutations[randomchoice];
                                        for(int i = 0; i < allattempts.size(); i++) {
                                                if(attempt == allattempts[i]) {
                                                        possiblepermutations[std::rand() % possiblepermutations.size()];
                                                        i = 0;
                                                }
                                        }
                                        allattempts.push_back(attempt);
                                        numofattempts++;
                                }
                        }

                }
                if(increment == true) {
                        if(firstattempt == true) {
                                for (int i = 0; i < length; i++) {
                                        attempt.push_back(0);

                                }
                                tempattempt = attempt;
                                firstattempt = false;
                        }
                        else{
                                tempattempt[elementindex] = elementvalue;
                                attempt = tempattempt;

                        }

                }
        }


        void learn(std::vector<int>& attempt, int black_hits, int white_hits){
                if(elimination == true) {
                        if(firstattempt == true) {
                                createPossibilities(attempt, black_hits, white_hits);
                                firstattempt = false;
                        }

                        else{
                                eliminate(possiblepermutations,attempt,black_hits,white_hits);
                        }
                }

                if(permutation == true) {
                        if(black_hits_total < length) {
                                black_hits_total = black_hits + black_hits_total;
                                for(int i = 0; i < black_hits; i++) {
                                        permutatedcode.push_back(attempt[i]);
                                }
                        }
                        else{
                                if(possible == false)  {
                                        possible = true;
                                        createPossibilities(attempt,black_hits,white_hits);
                                }
                                if(possible == true) {
                                        eliminate(possiblepermutations,attempt,black_hits,white_hits);
                                }
                        }
                }

                if(increment == true) {
                        if((black_hits > prevbh)&&(elementvalue > 0)) { \
                                elementindex++;
                                elementvalue = 1;
                                for(int i = 0; i < whitehitelements.size(); i++) {
                                        if(whitehitelements[i] == elementvalue) {
                                                elementvalue = elementvalue+1;
                                                i = 0;
                                        }
                                }
                                prevbh = black_hits;
                                prevvector = attempt;
                                prevwh = white_hits;

                        }
                        else if((black_hits == prevbh)&&(white_hits<=prevwh)&&(white_hits==0)) {
                                whitehitelements.push_back(elementvalue);
                                elementvalue++;
                                for(int i = 0; i < whitehitelements.size(); i++) {
                                        if(whitehitelements[i] == elementvalue) {
                                                elementvalue = elementvalue+1;
                                                i = 0;
                                        }
                                }
                                prevwh = white_hits;
                        }
                        else if(black_hits<prevbh) {

                                elementindex++;
                                tempattempt = prevvector;
                                elementvalue = 1;
                                for(int i = 0; i < whitehitelements.size(); i++) {
                                        if(whitehitelements[i] == elementvalue) {
                                                elementvalue = elementvalue+1;
                                                i = 0;
                                        }
                                }

                        }
                        else{

                                prevbh = black_hits;
                                prevwh = white_hits;
                                prevvector = attempt;
                                elementvalue = elementvalue+1;
                                for(int i = 0; i < whitehitelements.size(); i++) {
                                        if(whitehitelements[i] == elementvalue) {
                                                elementvalue = elementvalue+1;
                                                i = 0;
                                        }
                                }
                        }
                }


        }

        void createPossibilities(std::vector<int>& attempt, int black_hits, int white_hits) {

                std::vector<int> current(length, 0);
                std::vector<int> elements;

                for (int i = 0; i < num; i++) {
                        elements.push_back(i);
                }

                combinationRecursive(length, 0, current, elements,attempt, black_hits, white_hits);
        }

        void combinationRecursive(int length, int position, std::vector<int> &current, std::vector<int> &elements, std::vector<int>& attempt, int black_hits, int white_hits) {

                if (position >= length) {
                        give_relativefeedback(attempt,current,relativeblack,relativewhite);
                        if((relativeblack == black_hits) &&(relativewhite == white_hits)) {
                                possiblepermutations.push_back(current);

                        }
                        return;
                }
                for (int j = 0; j < elements.size(); ++j) {
                        current[position] = elements[j];
                        combinationRecursive(length, position + 1, current, elements, attempt,black_hits,white_hits);
                }

        }

        void eliminate(std::vector<std::vector<int> >& possiblepermutations,std::vector<int >& attempt,int black_hits, int white_hits){

                std::vector<std::vector<int> > tempset = possiblepermutations;
                possiblepermutations.clear();

                for(int i = 0; i < tempset.size(); i++) {
                        give_relativefeedback(attempt,tempset[i],relativeblack,relativewhite);
                        if((relativeblack == black_hits) &&(relativewhite == white_hits)) {
                                possiblepermutations.push_back(tempset[i]);
                        }
                }
        }



        void give_relativefeedback(const std::vector<int>& sequence,const std::vector<int>& attempt, int black_hits, int white_hits){
                relativeblack = 0;
                relativewhite = 0;
                int totalrelative = 0;
                std::vector<int>tempsequence;
                tempsequence = sequence;
                for(int i = 0; i < attempt.size(); i++) {
                        if(sequence[i] == attempt[i]) {
                                relativeblack++;
                        }
                }

                for(int i = 0; i < tempsequence.size(); i++) {
                        bool hasalreadybeenhit = false;
                        for(int j = 0; j < tempsequence.size(); j++) {
                                if((tempsequence[j]== attempt[i])&&(hasalreadybeenhit == false)) {
                                        tempsequence[j] = -1;
                                        totalrelative++;
                                        hasalreadybeenhit = true;
                                }

                        }
                }
                relativewhite = totalrelative - relativeblack;
        }

        void permute(std::vector<int>& attempt){
                int n = std::rand() % length;
                int n2 = std::rand() % length;

                std::vector<int> tmp;
                tmp = attempt;

                attempt[n] = tmp[n2];
                attempt[n2] = tmp[n];
        }
};

int main(){
        /// our program uses random features so we need to call the function setting a random seed
        set_random_seed();

        int length, num;
        std::cout << "enter length of sequence and number of possible values:" << std::endl;
        std::cin >> length >> num;

        mm_solver solver;
        /// we declare an object of type mm_solver
        solver.init(length, num);
        /// we initialise the values for length and num

        mm_code_maker maker;
        /// we declare an object of type mm_code_maker
        maker.init(length, num);
        /// we initialise the values for length and num

        maker.generate_sequence();
        /// we generate a random sequence

        int black_hits=0, white_hits=0;

        int attempts_limit = 5000;
        /// just some number to limit the number of attempts

        int attempts = 0;


        while((black_hits < length) && (attempts < attempts_limit)) {
                std::vector<int> attempt;
                solver.create_attempt(attempt);
                /// the solver creates an attempt
                maker.give_feedback(attempt, black_hits, white_hits);
                /// we ask for feedback for the attempt

                /// we print the attempt
                std::cout << "attempt: " << std::endl;
                for(int i = 0; i < attempt.size(); i++) {
                        std::cout << attempt[i] << " ";
                }
                std::cout << std::endl;

                /// we print the feedback
                std::cout << "black pegs: " << black_hits << " " << " white pegs: " << white_hits << std::endl;

                /// we give the feedback to the solver so that it can learn
                solver.learn(attempt, black_hits, white_hits);

                attempts++;
        }

        if(black_hits == length) {
                std::cout << "the solver has found the sequence in " << attempts << " attempts" << std::endl;
        }
        else{
                std::cout << "after " << attempts << " attempts still no solution" << std::endl;
        }

        std::cout << "the sequence generated by the code maker was:" << std::endl;
        for(int i = 0; i < maker.sequence.size(); i++) {
                std::cout << maker.sequence[i] << " ";
        }


        std::cout << std::endl;
        return 0;
}


void set_random_seed(){
        std::srand(std::time(0));
}

int randn(int n){
        return std::rand() % n;
}
