#include "globals.h"
#include "jgame.h"

f32 j_cos(f32 x) {
        int i;
        f32 result = 1.0;
        f32 term = 1.0;

        for (i = 1; i <= 10; i++) {
                term *= -x * x / ((2 * i) * (2 * i - 1));
                result += term;
        }

        return result;
}

f32 j_sin(f32 x) {
        int i;
        f32 result = x;
        f32 term = x;

        for (i = 2; i <= 10; i += 2) {
                term *= -x * x / ((i + 1) * i);
                result += term;
        }

        return result;
}

void mat4x4_identity(mat4x4 mat) {
        for (int i = 0; i < 16; i++) {
                mat[i] = (i % 5 == 0) ? 1.0f : 0.0f;
        }
}

void mat4x4_scale(mat4x4 mat4, vec3 vec3) {
	mat4[0]  *= vec3.x;
	mat4[5]  *= vec3.y;
	mat4[10] *= vec3.z;
}

void mat4x4_translate(mat4x4 mat4, vec3 vec3) {
	mat4[3]  += vec3.x;
	mat4[7]  += vec3.y;
	mat4[11] += vec3.z;
}

f32 j_sinf(f32 x) {
        f32 result = 0.0f;
        f32 term = x;
        f32 x_squared = x * x;
        int sign = 1;
        int factorial = 1;

        for (int i = 1; i <= 10; ++i) {  // Adjust the number of iterations for desired precision
                result += sign * term;
                sign *= -1;
                factorial *= (2 * i) * (2 * i + 1);
                term *= x_squared / factorial;
        }

        return result;
}

f32 j_sqrtf(f32 x) {
        if (x == 0.0f || x == 1.0f)
                return x;

        f32 result = x;
        f32 precision = 0.00001f; // adjust this for desired precision
        while ((result - x / result) > precision) {
                result = 0.5f * (result + x / result);
        }
        return result;
}


void mat4x4_rotate(mat4x4 mat4, vec3 axis, f32 angle_in_degrees) {
        f32 radians = angle_in_degrees * (J_PI / 180.0f);
        f32 c = j_cos(radians);
        f32 s = j_sinf(radians);
        f32 omc = 1.0f - c;

        f32 x = axis.x;
        f32 y = axis.y;
        f32 z = axis.z;

        // Normalize the axis
        f32 length = sqrtf(x * x + y * y + z * z);
        if (length != 0.0f) {
                x /= length;
                y /= length;
                z /= length;
        }

        f32 rotationMatrix[16] = {
            c + x * x * omc,       y * x * omc + z * s,  z * x * omc - y * s,  0.0f,
            x * y * omc - z * s,  c + y * y * omc,       z * y * omc + x * s,  0.0f,
            x * z * omc + y * s,  y * z * omc - x * s,  c + z * z * omc,       0.0f,
            0.0f,                  0.0f,                  0.0f,                  1.0f
        };

        // Multiply the rotation matrix with the input matrix
        f32 result[16];
        for (int i = 0; i < 4; ++i) {
                for (int j = 0; j < 4; ++j) {
                        result[i * 4 + j] = 0.0f;
                        for (int k = 0; k < 4; ++k) {
                                result[i * 4 + j] += mat4[k * 4 + j] * rotationMatrix[i * 4 + k];
                        }
                }
        }

        // Copy the result back to the original matrix
        for (int i = 0; i < 16; ++i) {
                mat4[i] = result[i];
        }
}

float normalize_value(float value, float src_max, float dest_max) {
	// Assume 0.0 is min value
	float intermediate = value / src_max;
	float result = dest_max * intermediate;
	return result;
}

float normalize_screen_px_to_ndc(int value, int max) {
	float this1 = (float)(value) / (float)(max);
	float this2 = 2.0f * this1;
	float res = -1.0f + this2;
	return res;
}

s64 vw_to_screen_px(float vw) {
	float result = vw * (float)user_settings.window_width_px * 0.01f;
	return result;
}

s64 vh_to_screen_px(float vh) {
	float result = vh * (float)user_settings.window_height_px * 0.01f;
	return result;
}
