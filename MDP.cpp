#include <iostream>
#include <random>
#include <vector>
#include <ctime>
#include <cstring>
#include <string>

constexpr std::size_t N_STATES = 10;
constexpr std::size_t N_ACTIONS = 5;
constexpr double GAMMA = 0.75;


/* p(s' | s, a) drawn from an exponential distribution, and then normalized
 * p(s', r | s, a) = p(r | s', s, a) * p(s' | s, a)
 * p(r | s', s, a) will follow a normal distribution
 * E[r | s', s, a] is drawn from a normal distribution
 * stddev[r | s', s, a] is drawn from an exponential distribution
 */
static std::vector<std::vector<std::discrete_distribution<std::size_t> > > init_probs(std::size_t STATES, std::size_t ACTIONS, std::default_random_engine &detgen, double PROB_BETA)
{
    std::exponential_distribution<double> probd(PROB_BETA);
    std::vector<std::vector<std::discrete_distribution<std::size_t> > > probs(STATES, std::vector<std::discrete_distribution<std::size_t> >(ACTIONS));

    std::vector<double> weights(STATES);
    for(std::size_t s = 0; s < STATES; ++s)
    {
        for(std::size_t a = 0; a < ACTIONS; ++a)
        {
            for(std::size_t ss = 0; ss < STATES; ++ss)
            {
                weights[ss] = probd(detgen);
            }
            probs[s][a] = std::discrete_distribution<std::size_t>(weights.begin(), weights.end());
        }
    }
    return probs;
}

static std::vector<std::vector<std::vector<std::normal_distribution<double> > > > init_rewards(std::size_t STATES, std::size_t ACTIONS, std::default_random_engine &detgen, double VAR_BETA, double EXP_MEAN, double EXP_DEV)
{
    std::exponential_distribution<double> vard(VAR_BETA);
    std::normal_distribution<double> expd(EXP_MEAN, EXP_DEV);

    std::vector<std::vector<std::vector<std::normal_distribution<double> > > > rewards(N_STATES, std::vector<std::vector<std::normal_distribution<double> > >(N_ACTIONS, std::vector<std::normal_distribution<double> >(N_STATES)));

    for(std::size_t s = 0; s < STATES; ++s)
    {
        for(std::size_t a = 0; a < ACTIONS; ++a)
        {
            for(std::size_t ss = 0; ss < STATES; ++ss)
            {
                rewards[s][a][ss] = std::normal_distribution<double>(expd(detgen), vard(detgen));
            }
        }
    }
    return rewards;
}

class MDP {
    private:
        const static double PROB_BETA;
        const static double VAR_BETA;
        const static double EXP_MEAN;
        const static double EXP_DEV;

        static std::default_random_engine detgen;
        static std::default_random_engine nondetgen;

        const std::size_t N_STATES, N_ACTIONS;
        const double mgamma;
        /* Conceptually these should be const, but stateless distributions are inefficient and tricky to implement
         * so, the C++ STL implementations of these distributions have operator() as non-const
         * See: https://stackoverflow.com/questions/16008271/why-are-c11-random-distributions-mutable
         */
        /* const */ std::vector<std::vector<std::discrete_distribution<std::size_t> > > probs;
        /* const */ std::vector<std::vector<std::vector<std::normal_distribution<double> > > > rewards;

        std::size_t mstate;
        double mReturn;
    public:
        MDP(std::size_t start_state, std::size_t N_STATES, std::size_t N_ACTIONS, double gamma): N_STATES(N_STATES), N_ACTIONS(N_ACTIONS), mstate(start_state), probs(init_probs(N_STATES, N_ACTIONS, detgen, PROB_BETA)), rewards(init_rewards(N_STATES, N_ACTIONS, detgen, VAR_BETA, EXP_MEAN, EXP_DEV)), mgamma(gamma), mReturn(0){}

        std::size_t state() const
        {
            return mstate;
        }

        double gamma() const
        {
            return mgamma;
        }

        double num_states() const
        {
            return N_STATES;
        }

        double num_actions() const
        {
            return N_ACTIONS;
        }

        double Return() const
        {
            return mReturn;
        }

        double transition(std::size_t action)
        {
            std::size_t next_state = probs[mstate][action](detgen);
            double reward = rewards[mstate][action][next_state](detgen);
            mstate = next_state;
            mReturn = reward + mgamma*mReturn;
            return reward;
        }
};

const double MDP::PROB_BETA = 2.0;
const double MDP::VAR_BETA = 3.0;
const double MDP::EXP_MEAN = 0.0;
const double MDP::EXP_DEV = 1.0;
std::default_random_engine MDP::detgen(0);
std::default_random_engine MDP::nondetgen(time(nullptr));

static void print_usage(char **argv)
{
    std::cerr << "Usage\nTo get the number of states: " << argv[0] << " states\nTo get the number of actions: " << argv[0] << " actions\n";
    std::cerr << "To get the discount factor: " << argv[0] << " gamma\n" << "To start the MDP: " << argv[0] << " <starting state>" << std::endl;
}

int main(int argc, char **argv)
{
    if(argc != 2)
    {
        print_usage(argv);
        return -1;
    }
    
    if(strcmp(argv[1], "states") == 0)
    {
        std::cout << N_STATES << std::endl;
        return 0;
    }
    
    if(strcmp(argv[1], "actions") == 0)
    {
        std::cout << N_ACTIONS << std::endl;
        return 0;
    }

    if(strcmp(argv[1], "gamma") == 0)
    {
        std::cout << GAMMA << std::endl;
        return 0;
    }

    std::size_t istate;
    std::size_t chars_read; // stoi parses 20ab as 20, therefore need an extra check. boost/lexical_cast would have been better

    try
    {
        istate = std::stoi(argv[1], &chars_read);
    }
    catch(const std::invalid_argument &)
    {
        print_usage(argv);
        return -1;
    }

    if(chars_read < strlen(argv[1]))
    {
        print_usage(argv);
        return -1;
    }

    MDP mdp(istate, N_STATES, N_ACTIONS, GAMMA);

    if(istate >= mdp.num_states())
    {
        std::cerr << istate << " is not a valid state" << std::endl;
        return -1;
    }

    while(true)
    {
        std::cout << "Current state: " << mdp.state() << '\n';
        std::cout << "Current return: " << mdp.Return() << '\n';
        std::cout << "Choose an action: ";
        std::cout.flush();
        std::size_t action;
        if(std::cin >> action)
        {
            if(action >= mdp.num_actions())
            {
                std::cerr << action << " is not a valid action" << std::endl;
                return -1;
            }
            std::cout << "Reward: " << mdp.transition(action) << std::endl;
        }
        else
        {
            std::cout << std::endl;
            break;
        }
    }
    return 0;
}
