#pragma once

bool TestConstructors()
{
    sp::owning_ptr<int> optr1;

    auto ptr1 = sp::make_owning<int>();
    (void)ptr1;

    auto ptr2 = sp::make_owning<int[]>(10);
    (void)ptr2;
    return true;
}