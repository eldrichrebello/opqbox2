//
// Created by tusk on 8/29/16.
//

#include "AntialiasDownsamplingFilter.hpp"

static float filter_taps[ANTIALIASDOWNSAMPLINGFILTER_TAP_NUM] = {
        -0.0029035232693220584,
        0.0035345367952893886,
        0.0035907746457514884,
        0.0043643484563067065,
        0.005497639037982969,
        0.006801354610866262,
        0.0081535506802464,
        0.009447319657530843,
        0.01059473509764183,
        0.01150408287409889,
        0.012098220284890139,
        0.012309275978865917,
        0.012087962251619417,
        0.01140876944766184,
        0.010270515813059624,
        0.008703279910729032,
        0.006764278040358339,
        0.004540020112676126,
        0.0021405654065154915,
        -0.00030548111449898194,
        -0.0026574251592209213,
        -0.004773038293623832,
        -0.006515854483935016,
        -0.007767184474724106,
        -0.008433803159853414,
        -0.00845697291210588,
        -0.007818750371874818,
        -0.006545001955810458,
        -0.00470770865065877,
        -0.0024211572326784596,
        0.00016196123292518236,
        0.002860034050166745,
        0.005473057832243912,
        0.007795410621743866,
        0.009630915216214099,
        0.010806041001926038,
        0.011185234466691606,
        0.010681736494237488,
        0.009268117673420211,
        0.006982180758783921,
        0.003928693882239496,
        0.0002783182741311163,
        -0.00374077489074238,
        -0.00785207328267802,
        -0.01174713132852428,
        -0.015102264804439719,
        -0.01759823539293515,
        -0.01894159160659899,
        -0.018882279113277008,
        -0.017234407500628993,
        -0.013888041136417768,
        -0.008822626879775457,
        -0.0021117760038726874,
        0.006077038743358479,
        0.01548417127873539,
        0.025774585252019182,
        0.036545846419347806,
        0.04735648825213341,
        0.057743389284863965,
        0.0672457841055638,
        0.07544259581919423,
        0.08194044433790082,
        0.08646028109190149,
        0.08877017518770029,
        0.08877017518770029,
        0.08646028109190149,
        0.08194044433790082,
        0.07544259581919423,
        0.0672457841055638,
        0.057743389284863965,
        0.04735648825213341,
        0.036545846419347806,
        0.025774585252019182,
        0.01548417127873539,
        0.006077038743358479,
        -0.0021117760038726874,
        -0.008822626879775457,
        -0.013888041136417768,
        -0.017234407500628993,
        -0.018882279113277008,
        -0.018941591606598987,
        -0.01759823539293515,
        -0.015102264804439719,
        -0.01174713132852428,
        -0.00785207328267802,
        -0.00374077489074238,
        0.0002783182741311163,
        0.003928693882239496,
        0.006982180758783921,
        0.009268117673420211,
        0.010681736494237485,
        0.011185234466691604,
        0.010806041001926038,
        0.009630915216214099,
        0.007795410621743866,
        0.005473057832243912,
        0.002860034050166745,
        0.0001619612329251828,
        -0.0024211572326784596,
        -0.004707708650658773,
        -0.006545001955810458,
        -0.007818750371874818,
        -0.00845697291210588,
        -0.008433803159853414,
        -0.007767184474724106,
        -0.0065158544839350125,
        -0.004773038293623832,
        -0.0026574251592209213,
        -0.00030548111449898194,
        0.0021405654065154915,
        0.004540020112676126,
        0.006764278040358339,
        0.008703279910729032,
        0.010270515813059624,
        0.01140876944766184,
        0.01208796225161942,
        0.012309275978865917,
        0.012098220284890139,
        0.01150408287409889,
        0.01059473509764183,
        0.009447319657530843,
        0.0081535506802464,
        0.006801354610866262,
        0.005497639037982969,
        0.004364348456306708,
        0.0035907746457514884,
        0.0035345367952893886,
        -0.002903523269322058
};

void AntialiasDownsamplingFilter_init(AntialiasDownsamplingFilter* f) {
    int i;
    for(i = 0; i < ANTIALIASDOWNSAMPLINGFILTER_TAP_NUM; ++i)
        f->history[i] = 0;
    f->last_index = 0;
}

void AntialiasDownsamplingFilter_put(AntialiasDownsamplingFilter* f, float input) {
    f->history[(f->last_index++) & 127] = input;
}

float AntialiasDownsamplingFilter_get(AntialiasDownsamplingFilter* f) {
    float acc = 0;
    int index = f->last_index, i;
    for(i = 0; i < ANTIALIASDOWNSAMPLINGFILTER_TAP_NUM; ++i) {
        acc += f->history[(index--) & 127] * filter_taps[i];
    };
    return acc;
}