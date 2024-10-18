#include <iostream>
#include <sstream>

using namespace std;

template<typename Cin, typename Cout>
void solve(Cin& cin, Cout& cout){
}
template<typename Cin, typename Cout>
void naive(Cin& cin, Cout& cout){
}
const bool stress = true;
int main(){
    if (!stress) solve(cin, cout);
    else{
        while(1){
            stringstream cin_solve, cout_solve, cout_naive;
            { // gen input
                cin_solve << 4;
            }
            stringstream cin_naive(cin_solve.str());
            solve(cin_solve, cout_solve);
            naive(cin_naive, cout_naive);
            if (cout_solve.str() != cout_naive.str()){
                cout << "TEST:\n" << cin_solve.str() << "\nANSWER: \n" << cout_solve.str() << "\nRIGHT ANSWER: \n" << cout_naive.str();
                exit(1);
            }
        }
    }
    return 0;
}