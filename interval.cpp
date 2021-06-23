#include "interval.h"
#include <iostream>

bool interval::in(mpz_class value)
{
    return ((this->a <=  value) && (value <= this->b));
}

void interval::set_interval(mpz_class a, mpz_class b)
{
    this->a = a;
    this->b = b;
}

mpz_class interval::size()
{
    return (this->b - this->a);
}

bool operator == (interval ab, interval cd)
{
    return ((ab.a == cd.a) && (ab.b == cd.b));
}

bool operator < (interval ab, interval cd)
{
    return (ab.a < cd.a);
}

bool operator <= (interval ab, interval cd)
{
    return (ab.a <= cd.a);
}


interval reunion(interval ab, interval cd)
{
    interval reunion;
    bool flag_a = cd.in(ab.a);
    bool flag_b = cd.in(ab.b);
    bool flag_c = ab.in(cd.a);
    bool flag_d = ab.in(cd.b);

    if(!flag_a && !flag_b && !flag_c && !flag_d) throw "Disjucte.";

    if(flag_a && flag_b)
    {
        reunion.a = cd.a;
        reunion.b = cd.b;
    }

    if(flag_a && !flag_b)
    {
        reunion.a = cd.a;
        reunion.b = ab.b;
    }

    if(!flag_a && flag_b)
    {
        reunion.a = ab.a;
        reunion.b = cd.b;
    }

    if(flag_c && flag_d)
    {
        reunion.a = ab.a;
        reunion.b = ab.b;
    }

    return reunion;
}

void multiple_reunion(vector<interval> &input)
{
    sort(input.begin(), input.end());
    uint32_t i = 0;

    while(i != input.size()-1)
    {
        try
        {
            input[i+1] = reunion(input[i], input[i+1]);
            input.erase(input.begin() + i);
        }
        catch(const char *e)
        {
            i++;
        }
    }

}


void safe_interval_insert(vector <interval> &M_new, interval in)
{
    mpz_class lb, rb;
    mpz_class lb_to_subsitute;
    mpz_class rb_to_subsitute;
    int left_index_to_substitute = -1;
    int right_index_to_substitute = -1;
    int a_in_some_interval = 0;
    int b_in_some_interval = 0;
    int ctr = 0;
    for(auto it = std::begin(M_new); it != std::end(M_new); it++)
    {
        if(it->in(in.a))
            {
                lb = it->a;
                left_index_to_substitute = ctr;
                a_in_some_interval = 1;
            }
         if(it->in(in.b))
            {
                rb = it->b;
                right_index_to_substitute = ctr;
                b_in_some_interval = 1;
            }
        ctr++;
    }

    cout << "a_in_some_interval " << a_in_some_interval << endl;
    cout << "b_in_some_interval " << b_in_some_interval << endl;

    if(a_in_some_interval && b_in_some_interval)
    {
        if(left_index_to_substitute < right_index_to_substitute)
        {
            M_new.erase(M_new.begin() + left_index_to_substitute, M_new.begin() + right_index_to_substitute);
            interval new_interval(lb, rb);
            M_new[left_index_to_substitute] = new_interval;
        }
        cout << "case 1 " << endl;
        return;
    }


    if(!a_in_some_interval)
    {
        ctr = 0;
        for(auto it = std::begin(M_new); it != std::end(M_new); it++)
        {
            if(in.a < it->a)
            {
                lb = in.a;
                left_index_to_substitute = ctr;
                break;
            }

            ctr++;
        }

        if(ctr == M_new.size())
            {
                left_index_to_substitute = ctr;
                lb = in.a;
            }
    }


    for(auto it = std::end(M_new) - 1; it != std::begin(M_new) - 1; it--)
        cout << "a " << it->a << " b " << it->b << endl;


    if(!b_in_some_interval)
    {
        ctr = M_new.size() - 1;
        for(auto it = std::end(M_new) - 1; it != std::begin(M_new) - 1; it--)
        {
            //cout << "a " << it->a << " b " << it->b << endl;
            if(in.b > it->b)
            {
                rb = in.b;
                right_index_to_substitute = ctr + 1;
                break;
            }

            ctr--;

        }

         if(ctr == -1)
                {
                    right_index_to_substitute = 0;
                    rb = in.b;
                }

    }

    cout << "left_index_to_substitute " << left_index_to_substitute << endl;
    cout << "right_index_to_substitute " << right_index_to_substitute << endl;

    if(!a_in_some_interval && b_in_some_interval)
    {
        if(left_index_to_substitute == right_index_to_substitute)
        {
            interval new_interval(lb, rb);
            M_new[left_index_to_substitute] = new_interval;
        }
        else
        {
            M_new.erase(M_new.begin() + left_index_to_substitute, M_new.begin() + right_index_to_substitute);
            interval new_interval(lb, rb);
            M_new[left_index_to_substitute] = new_interval;
        }

        return;
    }

    if(a_in_some_interval && !b_in_some_interval)
    {
        if(left_index_to_substitute == right_index_to_substitute)
        {
            interval new_interval(lb, rb);
            M_new[left_index_to_substitute] = new_interval;
        }
        else
        {
            //if(right_index_to_substitute == M_new.size())
                //right_index_to_substitute = right_index_to_substitute - 1;
            M_new.erase(M_new.begin() + left_index_to_substitute, M_new.begin() + right_index_to_substitute - 1);
            interval new_interval(lb, rb);
            M_new[left_index_to_substitute] = new_interval;
        }

        return;
    }


    cout << "lb " << lb << endl;
    cout << "rb " << rb << endl;
    if(left_index_to_substitute == right_index_to_substitute)
    {
        interval new_interval(lb, rb);
        if(left_index_to_substitute == M_new.size())
            M_new.push_back(new_interval);
        else if(left_index_to_substitute == 0)
            M_new.insert(M_new.begin(), new_interval);
        else
            M_new.insert(M_new.begin() + left_index_to_substitute, new_interval);

        return;

    }
    else
    {
        interval new_interval(lb, rb);
        if(right_index_to_substitute == M_new.size())
        {
            if(left_index_to_substitute == 0)
            {
                M_new.erase(M_new.begin(), M_new.end());
                cout << M_new.size() << endl;
                M_new.insert(M_new.begin(), new_interval);
            }
            else
            {
                M_new.erase(M_new.begin() + left_index_to_substitute, M_new.end());
                for(auto it = std::end(M_new) - 1; it != std::begin(M_new) - 1; it--)
                    cout << it->a << " " << it->b << endl;
                M_new.insert(M_new.begin() + left_index_to_substitute, new_interval);
            }
        }
        else
        {
            if(left_index_to_substitute == 0)
            {
                M_new.erase(M_new.begin(), M_new.begin() + right_index_to_substitute - 1);
                M_new[0] = new_interval;
            }
            else
            {
                M_new.erase(M_new.begin() + left_index_to_substitute, M_new.begin() + right_index_to_substitute - 1);
                M_new[left_index_to_substitute] = new_interval;
            }
        }




    }




}

void break_an_interval(vector <interval> &M_new, interval in)
{
    int ctr = 0;
    for(auto it = std::begin(M_new); it != std::end(M_new); it++)
    {
        if(it->in(in.a) && it->in(in.b))
        {
            M_new.erase(M_new.begin() + ctr, M_new.begin() + ctr + 1);
            M_new.insert(M_new.begin() +  ctr, in);
            //M_new[ctr] = in;
            break;
        }

        ctr++;
    }



}



