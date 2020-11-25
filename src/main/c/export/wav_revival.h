#pragma once

void voice_over(
        const char* orig_path,
        const char* voice_path,
        const char* output_path,
        double attack,
        double release,
        double silence,
        double threshold,
        double gain);
