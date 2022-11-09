//
// Created by Alessandro Budroni on 07/09/2022.
//

#ifndef PLAYAVX2_FISHER_YATES_H
#define PLAYAVX2_FISHER_YATES_H

#include <stdint.h>
#include "utils.h"

typedef uint16_t perm_t[PARAM_N1];

int set_random_with_bound_for_permutation(perm_t p, const uint16_t rnd_buff[CHUNK_RND_U16_LENGTH]);
void sample_random_chunk( uint8_t rnd_buff[CHUNK_RND_BYTES_LENGTH], uint8_t expanded_seed[SEED_BYTES + 2]);
void perm_set_random_sendrier_ct(perm_t p, uint8_t seed[SEED_BYTES]);
void perm_set_random_sendrier_ternary(perm_t p, uint8_t seed[SEED_BYTES]);
void perm_set_random_non_ct(perm_t p, uint8_t seed[SEED_BYTES]);

int verify_permutation(const perm_t p);

static const uint32_t fisher_yates_sampling_max[] = {
        64376, 64324, 65508, 65455, 65402, 65349, 65296, 65243, 65190, 65137, 65084, 65031, 64978, 64925, 64872, 64819, 64766, 64713, 64660, 64607, 64554, 64501, 64448, 64395, 64342, 65502, 65448, 65394, 65340, 65286, 65232, 65178, 65124, 65070, 65016, 64962, 64908, 64854, 64800, 64746, 64692, 64638, 64584, 64530, 64476, 64422, 64368, 65505, 65450, 65395, 65340, 65285, 65230, 65175, 65120, 65065, 65010, 64955, 64900, 64845, 64790, 64735, 64680, 64625, 64570, 64515, 64460, 64405, 65520, 65464, 65408, 65352, 65296, 65240, 65184, 65128, 65072, 65016, 64960, 64904, 64848, 64792, 64736, 64680, 64624, 64568, 64512, 64456, 64400, 65493, 65436, 65379, 65322, 65265, 65208, 65151, 65094, 65037, 64980, 64923, 64866, 64809, 64752, 64695, 64638, 64581, 64524, 64467, 64410, 65482, 65424, 65366, 65308, 65250, 65192, 65134, 65076, 65018, 64960, 64902, 64844, 64786, 64728, 64670, 64612, 64554, 64496, 64438, 65490, 65431, 65372, 65313, 65254, 65195, 65136, 65077, 65018, 64959, 64900, 64841, 64782, 64723, 64664, 64605, 64546, 64487, 65520, 65460, 65400, 65340, 65280, 65220, 65160, 65100, 65040, 64980, 64920, 64860, 64800, 64740, 64680, 64620, 64560, 64500, 65514, 65453, 65392, 65331, 65270, 65209, 65148, 65087, 65026, 64965, 64904, 64843, 64782, 64721, 64660, 64599, 64538, 65534, 65472, 65410, 65348, 65286, 65224, 65162, 65100, 65038, 64976, 64914, 64852, 64790, 64728, 64666, 64604, 64542, 65520, 65457, 65394, 65331, 65268, 65205, 65142, 65079, 65016, 64953, 64890, 64827, 64764, 64701, 64638, 64575, 65536, 65472, 65408, 65344, 65280, 65216, 65152, 65088, 65024, 64960, 64896, 64832, 64768, 64704, 64640, 64576, 65520, 65455, 65390, 65325, 65260, 65195, 65130, 65065, 65000, 64935, 64870, 64805, 64740, 64675, 64610, 64545, 65472, 65406, 65340, 65274, 65208, 65142, 65076, 65010, 64944, 64878, 64812, 64746, 64680, 64614, 65526, 65459, 65392, 65325, 65258, 65191, 65124, 65057, 64990, 64923, 64856, 64789, 64722, 64655, 64588, 65484, 65416, 65348, 65280, 65212, 65144, 65076, 65008, 64940, 64872, 64804, 64736, 64668, 64600, 65481, 65412, 65343, 65274, 65205, 65136, 65067, 64998, 64929, 64860, 64791, 64722, 64653, 65520, 65450, 65380, 65310, 65240, 65170, 65100, 65030, 64960, 64890, 64820, 64750, 64680, 65533, 65462, 65391, 65320, 65249, 65178, 65107, 65036, 64965, 64894, 64823, 64752, 64681, 65520, 65448, 65376, 65304, 65232, 65160, 65088, 65016, 64944, 64872, 64800, 64728, 64656, 65481, 65408, 65335, 65262, 65189, 65116, 65043, 64970, 64897, 64824, 64751, 64678, 65490, 65416, 65342, 65268, 65194, 65120, 65046, 64972, 64898, 64824, 64750, 64676, 65475, 65400, 65325, 65250, 65175, 65100, 65025, 64950, 64875, 64800, 64725, 65512, 65436, 65360, 65284, 65208, 65132, 65056, 64980, 64904, 64828, 64752, 65527, 65450, 65373, 65296, 65219, 65142, 65065, 64988, 64911, 64834, 64757, 65520, 65442, 65364, 65286, 65208, 65130, 65052, 64974, 64896, 64818, 64740, 65491, 65412, 65333, 65254, 65175, 65096, 65017, 64938, 64859, 64780, 65520, 65440, 65360, 65280, 65200, 65120, 65040, 64960, 64880, 64800, 65529, 65448, 65367, 65286, 65205, 65124, 65043, 64962, 64881, 64800, 65518, 65436, 65354, 65272, 65190, 65108, 65026, 64944, 64862, 64780, 65487, 65404, 65321, 65238, 65155, 65072, 64989, 64906, 64823, 65520, 65436, 65352, 65268, 65184, 65100, 65016, 64932, 64848, 65535, 65450, 65365, 65280, 65195, 65110, 65025, 64940, 64855, 65532, 65446, 65360, 65274, 65188, 65102, 65016, 64930, 64844, 65511, 65424, 65337, 65250, 65163, 65076, 64989, 64902, 64815, 65472, 65384, 65296, 65208, 65120, 65032, 64944, 64856, 65504, 65415, 65326, 65237, 65148, 65059, 64970, 64881, 65520, 65430, 65340, 65250, 65160, 65070, 64980, 64890, 65520, 65429, 65338, 65247, 65156, 65065, 64974, 64883, 65504, 65412, 65320, 65228, 65136, 65044, 64952, 64860, 65472, 65379, 65286, 65193, 65100, 65007, 64914, 65518, 65424, 65330, 65236, 65142, 65048, 64954, 64860, 65455, 65360, 65265, 65170, 65075, 64980, 64885, 65472, 65376, 65280, 65184, 65088, 64992, 64896, 65475, 65378, 65281, 65184, 65087, 64990, 64893, 65464, 65366, 65268, 65170, 65072, 64974, 64876, 65439, 65340, 65241, 65142, 65043, 64944, 65500, 65400, 65300, 65200, 65100, 65000, 64900, 65448, 65347, 65246, 65145, 65044, 64943, 65484, 65382, 65280, 65178, 65076, 64974, 65508, 65405, 65302, 65199, 65096, 64993, 65520, 65416, 65312, 65208, 65104, 65000, 65520, 65415, 65310, 65205, 65100, 64995, 65508, 65402, 65296, 65190, 65084, 64978, 65484, 65377, 65270, 65163, 65056, 64949, 65448, 65340, 65232, 65124, 65016, 65509, 65400, 65291, 65182, 65073, 64964, 65450, 65340, 65230, 65120, 65010, 65490, 65379, 65268, 65157, 65046, 65520, 65408, 65296, 65184, 65072, 64960, 65427, 65314, 65201, 65088, 64975, 65436, 65322, 65208, 65094, 64980, 65435, 65320, 65205, 65090, 64975, 65424, 65308, 65192, 65076, 65520, 65403, 65286, 65169, 65052, 65490, 65372, 65254, 65136, 65018, 65450, 65331, 65212, 65093, 65520, 65400, 65280, 65160, 65040, 65461, 65340, 65219, 65098, 65514, 65392, 65270, 65148, 65026, 65436, 65313, 65190, 65067, 65472, 65348, 65224, 65100, 65500, 65375, 65250, 65125, 65520, 65394, 65268, 65142, 65532, 65405, 65278, 65151, 65536, 65408, 65280, 65152, 65532, 65403, 65274, 65145, 65520, 65390, 65260, 65130, 65500, 65369, 65238, 65107, 65472, 65340, 65208, 65076, 65436, 65303, 65170, 65526, 65392, 65258, 65124, 65475, 65340, 65205, 65070, 65416, 65280, 65144, 65486, 65349, 65212, 65075, 65412, 65274, 65136, 65469, 65330, 65191, 65520, 65380, 65240, 65100, 65424, 65283, 65142, 65462, 65320, 65178, 65494, 65351, 65208, 65520, 65376, 65232, 65088, 65395, 65250, 65105, 65408, 65262, 65116, 65415, 65268, 65121, 65416, 65268, 65120, 65411, 65262, 65113, 65400, 65250, 65534, 65383, 65232, 65512, 65360, 65208, 65484, 65331, 65178, 65450, 65296, 65142, 65410, 65255, 65520, 65364, 65208, 65469, 65312, 65155, 65412, 65254, 65508, 65349, 65190, 65440, 65280, 65527, 65366, 65205, 65448, 65286, 65526, 65363, 65200, 65436, 65272, 65505, 65340, 65175, 65404, 65238, 65464, 65297, 65520, 65352, 65184, 65403, 65234, 65450, 65280, 65493, 65322, 65532, 65360, 65188, 65394, 65221, 65424, 65250, 65450, 65275, 65472, 65296, 65490, 65313, 65504, 65326, 65514, 65335, 65520, 65340, 65522, 65341, 65520, 65338, 65514, 65331, 65504, 65320, 65490, 65305, 65472, 65286, 65450, 65263, 65424, 65236, 65394, 65205, 65360, 65513, 65322, 65472, 65280, 65427, 65234, 65378, 65520, 65325, 65464, 65268, 65404, 65207, 65340, 65471, 65272, 65400, 65526, 65325, 65448, 65246, 65366, 65484, 65280, 65395, 65508, 65302, 65412, 65520, 65312, 65417, 65520, 65310, 65410, 65508, 65296, 65391, 65484, 65270, 65360, 65448, 65534, 65317, 65400, 65481, 65262, 65340, 65416, 65490, 65268, 65339, 65408, 65475, 65250, 65314, 65376, 65436, 65494, 65265, 65320, 65373, 65424, 65473, 65520, 65286, 65330, 65372, 65412, 65450, 65486, 65520, 65280, 65311, 65340, 65367, 65392, 65415, 65436, 65455, 65472, 65487, 65500, 65511, 65520, 65527, 65532, 65535, 65536, 65535, 65532, 65527, 65520, 65511, 65500, 65487, 65472, 65455, 65436, 65415, 65392, 65367, 65340, 65311, 65520, 65486, 65450, 65412, 65372, 65330, 65520, 65473, 65424, 65373, 65320, 65494, 65436, 65376, 65314, 65475, 65408, 65339, 65490, 65416, 65340, 65481, 65400, 65534, 65448, 65360, 65484, 65391, 65508, 65410, 65520, 65417, 65520, 65412, 65508, 65395, 65484, 65366, 65448, 65526, 65400, 65471, 65340, 65404, 65464, 65520, 65378, 65427, 65472, 65513, 65360, 65394, 65424, 65450, 65472, 65490, 65504, 65514, 65520, 65522, 65520, 65514, 65504, 65490, 65472, 65450, 65424, 65394, 65532, 65493, 65450, 65403, 65520, 65464, 65404, 65505, 65436, 65526, 65448, 65527, 65440, 65508, 65412, 65469, 65520, 65410, 65450, 65484, 65512, 65534, 65400, 65411, 65416, 65415, 65408, 65395, 65520, 65494, 65462, 65424, 65520, 65469, 65412, 65486, 65416, 65475, 65526, 65436, 65472, 65500, 65520, 65532, 65536, 65532, 65520, 65500, 65472, 65436, 65514, 65461, 65520, 65450, 65490, 65520, 65424, 65435, 65436, 65427, 65520, 65490, 65450, 65509, 65448, 65484, 65508, 65520, 65520, 65508, 65484, 65448, 65500, 65439, 65464, 65475, 65472, 65455, 65518, 65472, 65504, 65520, 65520, 65504, 65472, 65511, 65532, 65535, 65520, 65487, 65518, 65529, 65520, 65491, 65520, 65527, 65512, 65475, 65490, 65481, 65520, 65533, 65520, 65481, 65484, 65526, 65472, 65520, 65536, 65520, 65534, 65514, 65520, 65490, 65482, 65493, 65520, 65505, 65502, 65508, 65520, 65535, 65500, 65513, 65520, 65518, 65504, 65520, 65516, 65532, 65520, 65518, 65520, 65520, 65512, 65527, 65520, 65520, 65518, 65505, 65536, 65534, 65520, 65511, 65520, 65529, 65520, 65525, 65520, 65527, 65516, 65520, 65520, 65531, 65520, 65535, 65536, 65535, 65534, 65533, 65532, 65527, 65530, 65529, 65536, 65534, 65532, 65535, 65536, 65535, 65536, 65536
};

#endif //PLAYAVX2_FISHER_YATES_H
