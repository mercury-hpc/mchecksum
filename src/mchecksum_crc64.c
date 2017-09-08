/*
 * Copyright (C) 2013-2017 Argonne National Laboratory, Department of Energy,
 *                    UChicago Argonne, LLC and The HDF Group.
 * All rights reserved.
 *
 * The full copyright notice, including terms governing use, modification,
 * and redistribution, is contained in the COPYING file that can be
 * found at the root of the source code distribution tree.
 */

#include "mchecksum_crc64.h"
#include "mchecksum_error.h"

#include <stdlib.h>
#if defined(MCHECKSUM_HAS_ISAL)
  #include <isa-l.h>
#endif

static int mchecksum_crc64_destroy(struct mchecksum_class *checksum_class);
static int mchecksum_crc64_reset(struct mchecksum_class *checksum_class);
static size_t mchecksum_crc64_get_size(struct mchecksum_class *checksum_class);
static int mchecksum_crc64_get(struct mchecksum_class *checksum_class,
        void *buf, size_t size, int finalize);
static int mchecksum_crc64_update(struct mchecksum_class *checksum_class,
        const void *data, size_t size);

static struct mchecksum_class mchecksum_crc64_g = {
        NULL,
        mchecksum_crc64_destroy,
        mchecksum_crc64_reset,
        mchecksum_crc64_get_size,
        mchecksum_crc64_get,
        mchecksum_crc64_update
};

#if !defined(MCHECKSUM_HAS_ISAL)
static const mchecksum_uint64_t table_[256] = {
    0x0000000000000000ULL, 0xB32E4CBE03A75F6FULL, 0xF4843657A840A05BULL,
    0x47AA7AE9ABE7FF34ULL, 0x7BD0C384FF8F5E33ULL, 0xC8FE8F3AFC28015CULL,
    0x8F54F5D357CFFE68ULL, 0x3C7AB96D5468A107ULL, 0xF7A18709FF1EBC66ULL,
    0x448FCBB7FCB9E309ULL, 0x0325B15E575E1C3DULL, 0xB00BFDE054F94352ULL,
    0x8C71448D0091E255ULL, 0x3F5F08330336BD3AULL, 0x78F572DAA8D1420EULL,
    0xCBDB3E64AB761D61ULL, 0x7D9BA13851336649ULL, 0xCEB5ED8652943926ULL,
    0x891F976FF973C612ULL, 0x3A31DBD1FAD4997DULL, 0x064B62BCAEBC387AULL,
    0xB5652E02AD1B6715ULL, 0xF2CF54EB06FC9821ULL, 0x41E11855055BC74EULL,
    0x8A3A2631AE2DDA2FULL, 0x39146A8FAD8A8540ULL, 0x7EBE1066066D7A74ULL,
    0xCD905CD805CA251BULL, 0xF1EAE5B551A2841CULL, 0x42C4A90B5205DB73ULL,
    0x056ED3E2F9E22447ULL, 0xB6409F5CFA457B28ULL, 0xFB374270A266CC92ULL,
    0x48190ECEA1C193FDULL, 0x0FB374270A266CC9ULL, 0xBC9D3899098133A6ULL,
    0x80E781F45DE992A1ULL, 0x33C9CD4A5E4ECDCEULL, 0x7463B7A3F5A932FAULL,
    0xC74DFB1DF60E6D95ULL, 0x0C96C5795D7870F4ULL, 0xBFB889C75EDF2F9BULL,
    0xF812F32EF538D0AFULL, 0x4B3CBF90F69F8FC0ULL, 0x774606FDA2F72EC7ULL,
    0xC4684A43A15071A8ULL, 0x83C230AA0AB78E9CULL, 0x30EC7C140910D1F3ULL,
    0x86ACE348F355AADBULL, 0x3582AFF6F0F2F5B4ULL, 0x7228D51F5B150A80ULL,
    0xC10699A158B255EFULL, 0xFD7C20CC0CDAF4E8ULL, 0x4E526C720F7DAB87ULL,
    0x09F8169BA49A54B3ULL, 0xBAD65A25A73D0BDCULL, 0x710D64410C4B16BDULL,
    0xC22328FF0FEC49D2ULL, 0x85895216A40BB6E6ULL, 0x36A71EA8A7ACE989ULL,
    0x0ADDA7C5F3C4488EULL, 0xB9F3EB7BF06317E1ULL, 0xFE5991925B84E8D5ULL,
    0x4D77DD2C5823B7BAULL, 0x64B62BCAEBC387A1ULL, 0xD7986774E864D8CEULL,
    0x90321D9D438327FAULL, 0x231C512340247895ULL, 0x1F66E84E144CD992ULL,
    0xAC48A4F017EB86FDULL, 0xEBE2DE19BC0C79C9ULL, 0x58CC92A7BFAB26A6ULL,
    0x9317ACC314DD3BC7ULL, 0x2039E07D177A64A8ULL, 0x67939A94BC9D9B9CULL,
    0xD4BDD62ABF3AC4F3ULL, 0xE8C76F47EB5265F4ULL, 0x5BE923F9E8F53A9BULL,
    0x1C4359104312C5AFULL, 0xAF6D15AE40B59AC0ULL, 0x192D8AF2BAF0E1E8ULL,
    0xAA03C64CB957BE87ULL, 0xEDA9BCA512B041B3ULL, 0x5E87F01B11171EDCULL,
    0x62FD4976457FBFDBULL, 0xD1D305C846D8E0B4ULL, 0x96797F21ED3F1F80ULL,
    0x2557339FEE9840EFULL, 0xEE8C0DFB45EE5D8EULL, 0x5DA24145464902E1ULL,
    0x1A083BACEDAEFDD5ULL, 0xA9267712EE09A2BAULL, 0x955CCE7FBA6103BDULL,
    0x267282C1B9C65CD2ULL, 0x61D8F8281221A3E6ULL, 0xD2F6B4961186FC89ULL,
    0x9F8169BA49A54B33ULL, 0x2CAF25044A02145CULL, 0x6B055FEDE1E5EB68ULL,
    0xD82B1353E242B407ULL, 0xE451AA3EB62A1500ULL, 0x577FE680B58D4A6FULL,
    0x10D59C691E6AB55BULL, 0xA3FBD0D71DCDEA34ULL, 0x6820EEB3B6BBF755ULL,
    0xDB0EA20DB51CA83AULL, 0x9CA4D8E41EFB570EULL, 0x2F8A945A1D5C0861ULL,
    0x13F02D374934A966ULL, 0xA0DE61894A93F609ULL, 0xE7741B60E174093DULL,
    0x545A57DEE2D35652ULL, 0xE21AC88218962D7AULL, 0x5134843C1B317215ULL,
    0x169EFED5B0D68D21ULL, 0xA5B0B26BB371D24EULL, 0x99CA0B06E7197349ULL,
    0x2AE447B8E4BE2C26ULL, 0x6D4E3D514F59D312ULL, 0xDE6071EF4CFE8C7DULL,
    0x15BB4F8BE788911CULL, 0xA6950335E42FCE73ULL, 0xE13F79DC4FC83147ULL,
    0x521135624C6F6E28ULL, 0x6E6B8C0F1807CF2FULL, 0xDD45C0B11BA09040ULL,
    0x9AEFBA58B0476F74ULL, 0x29C1F6E6B3E0301BULL, 0xC96C5795D7870F42ULL,
    0x7A421B2BD420502DULL, 0x3DE861C27FC7AF19ULL, 0x8EC62D7C7C60F076ULL,
    0xB2BC941128085171ULL, 0x0192D8AF2BAF0E1EULL, 0x4638A2468048F12AULL,
    0xF516EEF883EFAE45ULL, 0x3ECDD09C2899B324ULL, 0x8DE39C222B3EEC4BULL,
    0xCA49E6CB80D9137FULL, 0x7967AA75837E4C10ULL, 0x451D1318D716ED17ULL,
    0xF6335FA6D4B1B278ULL, 0xB199254F7F564D4CULL, 0x02B769F17CF11223ULL,
    0xB4F7F6AD86B4690BULL, 0x07D9BA1385133664ULL, 0x4073C0FA2EF4C950ULL,
    0xF35D8C442D53963FULL, 0xCF273529793B3738ULL, 0x7C0979977A9C6857ULL,
    0x3BA3037ED17B9763ULL, 0x888D4FC0D2DCC80CULL, 0x435671A479AAD56DULL,
    0xF0783D1A7A0D8A02ULL, 0xB7D247F3D1EA7536ULL, 0x04FC0B4DD24D2A59ULL,
    0x3886B22086258B5EULL, 0x8BA8FE9E8582D431ULL, 0xCC0284772E652B05ULL,
    0x7F2CC8C92DC2746AULL, 0x325B15E575E1C3D0ULL, 0x8175595B76469CBFULL,
    0xC6DF23B2DDA1638BULL, 0x75F16F0CDE063CE4ULL, 0x498BD6618A6E9DE3ULL,
    0xFAA59ADF89C9C28CULL, 0xBD0FE036222E3DB8ULL, 0x0E21AC88218962D7ULL,
    0xC5FA92EC8AFF7FB6ULL, 0x76D4DE52895820D9ULL, 0x317EA4BB22BFDFEDULL,
    0x8250E80521188082ULL, 0xBE2A516875702185ULL, 0x0D041DD676D77EEAULL,
    0x4AAE673FDD3081DEULL, 0xF9802B81DE97DEB1ULL, 0x4FC0B4DD24D2A599ULL,
    0xFCEEF8632775FAF6ULL, 0xBB44828A8C9205C2ULL, 0x086ACE348F355AADULL,
    0x34107759DB5DFBAAULL, 0x873E3BE7D8FAA4C5ULL, 0xC094410E731D5BF1ULL,
    0x73BA0DB070BA049EULL, 0xB86133D4DBCC19FFULL, 0x0B4F7F6AD86B4690ULL,
    0x4CE50583738CB9A4ULL, 0xFFCB493D702BE6CBULL, 0xC3B1F050244347CCULL,
    0x709FBCEE27E418A3ULL, 0x3735C6078C03E797ULL, 0x841B8AB98FA4B8F8ULL,
    0xADDA7C5F3C4488E3ULL, 0x1EF430E13FE3D78CULL, 0x595E4A08940428B8ULL,
    0xEA7006B697A377D7ULL, 0xD60ABFDBC3CBD6D0ULL, 0x6524F365C06C89BFULL,
    0x228E898C6B8B768BULL, 0x91A0C532682C29E4ULL, 0x5A7BFB56C35A3485ULL,
    0xE955B7E8C0FD6BEAULL, 0xAEFFCD016B1A94DEULL, 0x1DD181BF68BDCBB1ULL,
    0x21AB38D23CD56AB6ULL, 0x9285746C3F7235D9ULL, 0xD52F0E859495CAEDULL,
    0x6601423B97329582ULL, 0xD041DD676D77EEAAULL, 0x636F91D96ED0B1C5ULL,
    0x24C5EB30C5374EF1ULL, 0x97EBA78EC690119EULL, 0xAB911EE392F8B099ULL,
    0x18BF525D915FEFF6ULL, 0x5F1528B43AB810C2ULL, 0xEC3B640A391F4FADULL,
    0x27E05A6E926952CCULL, 0x94CE16D091CE0DA3ULL, 0xD3646C393A29F297ULL,
    0x604A2087398EADF8ULL, 0x5C3099EA6DE60CFFULL, 0xEF1ED5546E415390ULL,
    0xA8B4AFBDC5A6ACA4ULL, 0x1B9AE303C601F3CBULL, 0x56ED3E2F9E224471ULL,
    0xE5C372919D851B1EULL, 0xA26908783662E42AULL, 0x114744C635C5BB45ULL,
    0x2D3DFDAB61AD1A42ULL, 0x9E13B115620A452DULL, 0xD9B9CBFCC9EDBA19ULL,
    0x6A978742CA4AE576ULL, 0xA14CB926613CF817ULL, 0x1262F598629BA778ULL,
    0x55C88F71C97C584CULL, 0xE6E6C3CFCADB0723ULL, 0xDA9C7AA29EB3A624ULL,
    0x69B2361C9D14F94BULL, 0x2E184CF536F3067FULL, 0x9D36004B35545910ULL,
    0x2B769F17CF112238ULL, 0x9858D3A9CCB67D57ULL, 0xDFF2A94067518263ULL,
    0x6CDCE5FE64F6DD0CULL, 0x50A65C93309E7C0BULL, 0xE388102D33392364ULL,
    0xA4226AC498DEDC50ULL, 0x170C267A9B79833FULL, 0xDCD7181E300F9E5EULL,
    0x6FF954A033A8C131ULL, 0x28532E49984F3E05ULL, 0x9B7D62F79BE8616AULL,
    0xA707DB9ACF80C06DULL, 0x14299724CC279F02ULL, 0x5383EDCD67C06036ULL,
    0xE0ADA17364673F59ULL
};
#endif

/**
 * Initialize the CRC table.
 */
/* CRC-64 ECMA Poly is 0x42F0E1EBA9EA3693 -> Rev Poly is 0xC96C5795D7870F42 */
/*
#define POLYREV    0xC96C5795D7870F42ULL

static void
gen_table(void)
{
    unsigned int i, j;
    unsigned int col = 0;

    for (i = 0; i < 256; i++) {
        mchecksum_uint64_t part = i;

        for (j = 0; j < 8; j++) {
            if (part & 1)
                part = (part >> 1) ^ POLYREV;
            else
                part >>= 1;
        }
        col++;
        printf("0x%016lXULL, ", part);
        if (col == 2) {
            printf("\n");
            col = 0;
        }
    }
    printf("\n");
}
*/

/*---------------------------------------------------------------------------*/
int
mchecksum_crc64_init(struct mchecksum_class *checksum_class)
{
    int ret = MCHECKSUM_SUCCESS;

    if (!checksum_class) {
        MCHECKSUM_ERROR_DEFAULT("NULL checksum class");
        ret = MCHECKSUM_FAIL;
        goto done;
    }

    *checksum_class = mchecksum_crc64_g;

    checksum_class->data = malloc(sizeof(mchecksum_uint64_t));
    if (!checksum_class->data) {
        MCHECKSUM_ERROR_DEFAULT("Could not allocate private data");
        ret = MCHECKSUM_FAIL;
        goto done;
    }

    mchecksum_crc64_reset(checksum_class);

done:
    return ret;
}

/*---------------------------------------------------------------------------*/
static int
mchecksum_crc64_destroy(struct mchecksum_class *checksum_class)
{
    free(checksum_class->data);

    return MCHECKSUM_SUCCESS;
}

/*---------------------------------------------------------------------------*/
static int
mchecksum_crc64_reset(struct mchecksum_class *checksum_class)
{
    *(mchecksum_uint64_t*) checksum_class->data = ~(mchecksum_uint64_t)0;

    return MCHECKSUM_SUCCESS;
}

/*---------------------------------------------------------------------------*/
static size_t
mchecksum_crc64_get_size(struct mchecksum_class MCHECKSUM_UNUSED *checksum_class)
{
    return sizeof(mchecksum_uint64_t);
}

/*---------------------------------------------------------------------------*/
static int
mchecksum_crc64_get(struct mchecksum_class *checksum_class,
        void *buf, size_t size, int MCHECKSUM_UNUSED finalize)
{
    int ret = MCHECKSUM_SUCCESS;

    if (size < sizeof(mchecksum_uint64_t)) {
        MCHECKSUM_ERROR_DEFAULT("Buffer is too small to store checksum");
        ret = MCHECKSUM_FAIL;
        goto done;
    }

    *(mchecksum_uint64_t*) buf = *(mchecksum_uint64_t*) checksum_class->data;

done:
    return ret;
}

/*---------------------------------------------------------------------------*/
static int
mchecksum_crc64_update(struct mchecksum_class *checksum_class,
        const void *data, size_t size)
{
    mchecksum_uint64_t *state = (mchecksum_uint64_t*) checksum_class->data;
#if defined(MCHECKSUM_HAS_ISAL)
    *state = ~crc64_ecma_refl(~*state, (const unsigned char *) data, size);
#else
    const unsigned char *cur = (const unsigned char *) data;
    const unsigned char *end = cur + size;

    while (cur < end)
        *state = ((*state >> 8) ^ table_[(*state ^ *cur++) & 0xff]);
#endif

    return MCHECKSUM_SUCCESS;
}
