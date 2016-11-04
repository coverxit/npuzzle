/**
 * \file CodeTiming.hpp
 * \brief Helper class for code timing.
 */
#ifndef __CODE_TIMING__
#define __CODE_TIMING__

//! Helper class for code timing.
class CodeTiming
{
public:
    /**
     * \brief Run a function and evaluate its execution time.
     * \param func The function to be called.
     * \param args The arguments to be passed into \c func.
     * \return A \c std::pair with the first element as the return value of \c func,
     *         and the second element to be the elapsed time.
     */
    template <class Callable, class... Arguments>
    static std::pair<typename std::result_of<Callable(Arguments...)>::type,
            std::chrono::duration<double>> Run(Callable&& func, Arguments&&... args)
    {
        auto task = std::bind(std::forward<Callable>(func), std::forward<Arguments>(args)...);

        auto start = std::chrono::steady_clock::now();
        auto ret = task();
        auto end = std::chrono::steady_clock::now();

        return std::make_pair(ret, end - start);
    }

    /**
     * \brief Run a \c void function and evaluate its execution time.
     * @param func The function to be called.
     * @param args The arguments to be passed into \c func.
     * @return The elapsed time.
     */
    template <class Callable, class... Arguments>
    static std::chrono::duration<double> RunVoid(Callable&& func, Arguments&&... args)
    {
        auto task = std::bind(std::forward<Callable>(func), std::forward<Arguments>(args)...);

        auto start = std::chrono::steady_clock::now();
        task();
        auto end = std::chrono::steady_clock::now();

        return end - start;
    }

    /**
     * \brief Convert a \c std::chrono::duration into a user-friendly string.
     * \param diff The duration to be processed.
     * \return The user-readable string.
     */
    template <typename Duration>
    static std::string FriendlyTime(std::chrono::duration<Duration> diff)
    {
        using namespace std::chrono;
        std::stringstream stream;

        if (duration_cast<microseconds>(diff).count() < 1000)
            stream << duration_cast<microseconds>(diff).count() << " us";
        else if (duration_cast<milliseconds>(diff).count() < 1000)
            stream << duration_cast<milliseconds>(diff).count() << " ms";
        else
            stream << diff.count() << " s";

        return stream.str();
    }
};

#endif
