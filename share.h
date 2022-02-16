#pragma once

bool ivertex_had_not_been_visited(int& i, int& memorizebit)
{
    if (memorizebit & (1 << i)) return false;
    return true;
}

int the_number_of_bit_is_on(const int& memorizebit) {
    int result = 0;
    for (int k = 0; memorizebit >= (1 << k); ++k)
        if (memorizebit & (1 << k)) ++result;
    return result;
}

bool is_bit_all_on(int memorizebit, int N)
{
    return memorizebit == (1 << N) - 1;
}
