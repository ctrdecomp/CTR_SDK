TEXT_SECTION 0x00100000 (0x04000000 - 0x00100000)
{
    STUP_ENTRY 0x00100000
    {
        * (:gdef:__ctr_start, +FIRST)
        * (+RO-CODE)
    }
}

RO_SECTION  +0  ALIGN   0x1000
{
    RO +0
    {
        * (+RO-DATA)
    }
    STATIC_INIT  +0 
    {
        * (.static_init)
    }
    ET +0
    {
        * (.ARM.exidx)
        * (.ARM.extab)
    }
}

RW_SECTION  +0  ALIGN   0x1000
{
    RW +0
    {
        * (+RW-DATA)
    }
    ZI +0
    {
        * (+ZI)
    }
}

END_SECTION +ImageLength(ZI)    ALIGN   0x1000
{
    STATIC_END +0
    {
    	* (.nn.dummy)
    }
}

MODULE_INFO (0x04000000)
{
    .module_id (0x04000000)
    {
        * (.module_id)
    }
}