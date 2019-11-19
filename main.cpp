#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <chrono>


inline double get_jaro_distance(double len_left, double len_right, double m, double t) noexcept
{
    double dj = (m/len_left + m/len_right + (m - t)/m)/3;
    std::cout << "Jaro distance: " << dj << std::endl;
    return dj;
}

inline double get_jaro_winkler_distance(const double & dj, const int & prefix_len) noexcept
{
    double dw = dj + 0.1 * prefix_len * (1 - dj);
    std::cout << "Jaro-Winkler distance: " << dw << std::endl;
    return dw;
}

inline int get_prefix_len(const std::wstring & wl, const std::wstring & wr) noexcept
{
    int i = 0;
    for (; i < std::min(wl.length(), wr.length()); i++)
    {
        if (wl[i] != wr[i])
            break;
    }
    std::cout << "Match prefix length: " << i << std::endl;
    return i;
}


inline int get_match_count(const std::wstring & wl, const std::wstring & wr) noexcept
{
    int m = 0;
    auto match_len_max = std::max(wl.length(), wr.length())/2 - 1;
    for (int i = 0; i < wl.length(); i++)
    {
        auto j = std::max(static_cast<int>(i - match_len_max), 0);
        for (; j < wr.length() && j <= i + match_len_max ; j++)
        {
            if (wl[i] == wr[j])
            {
                m++;
                break;
            }
        }
    }
    std::cout << "Match count: " << m << std::endl;
    return m;
}

inline int get_transposition_count(const std::wstring & wl, const std::wstring & wr) noexcept
{
    int trans_count = 0;
    auto trans_len_max = std::max(wl.length(), wr.length())/2 - 1;
    
    if (trans_len_max <= 0)
        return trans_count;
    
    for (int i = 0; i < wl.length(); i++)
    {
        if (wl[i] != wr[i])
        {
            for (int j = 1; j <= trans_len_max && i + j < wl.length(); j++)
            {
                if (wl[i] == wr[i + j] && wl[i + j] == wr[i])
                {
                    trans_count++;
                    continue;
                }
            }
        }
    }

    std::cout << "Transposition count: " << trans_count << std::endl;
    return trans_count;
}

inline double get_jaro_winkler_distance(const std::wstring & wl, const std::wstring & wr) noexcept
{
    if (wl == wr)
    {
        return 1.0;
    }
    
    double dj, dw;
    int match_count, trans_count, prefix_len;
    
    prefix_len = get_prefix_len(wl, wr);
    
    if (wl.length() <= wr.length())
    {
        match_count = get_match_count(wl, wr);
        trans_count = get_transposition_count(wl, wr);
    }
    else
    {
        match_count = get_match_count(wr, wl);
        trans_count = get_transposition_count(wr, wl);
    }
    
    dj = get_jaro_distance(static_cast<double>(wl.length()), static_cast<double>(wr.length()), match_count, trans_count);
    dw = get_jaro_winkler_distance(dj, prefix_len);
    
    return dw;
}


int main(int argc, const char * argv[])
{
    std::wstring wl(L"приветствие");
    std::wstring wr(L"приветтвиеееес");

    double dw;
        
    auto t_start = std::chrono::high_resolution_clock::now();
    
    std::cout.precision(6);
    
    dw = get_jaro_winkler_distance(wl, wr);
    
    auto t_end = std::chrono::high_resolution_clock::now();
    double elapsed_time_ms = std::chrono::duration<double, std::micro>(t_end - t_start).count();
    std::cout << "Elapsed time (ms) : " << elapsed_time_ms << std::endl;
    
    return 0;
}
