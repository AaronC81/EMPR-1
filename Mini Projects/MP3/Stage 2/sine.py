#!env python3.6

from math import sin , pi

amplitude = 1023 # 1023 is the maximum for the DAC
steps = 10
amp_step = 1. / steps
amp_scale = 0
samples = 2**10 # must be power of two, 1024 ~= 1 second
hertz = 1

step = ((2 * pi) / samples) * hertz

f = open('sine.h' , 'w')

f.write('#define N_AMPLITUDES ')
f.write(str(steps))
f.write('\n')
f.write('#define N_SAMPLES ')
f.write(str(samples))
f.write('\n')
f.write('static const uint32_t sin_table[ N_AMPLITUDES ][ N_SAMPLES ] = {\n')

for j in range(steps):
    f.write("\t{\n")
    amp_scale += amp_step
    for i in range(samples):
        r = i * step # angle in radians
        s = sin(r)
        s = s + 1 # shift into range [0,2]
        s = s / 2 # divide into range [0,1]
        s = round(s * amplitude * amp_scale) # multiply by amplitude
        f.write('\t\t')
        f.write(str(s))
        f.write(' ,\n')
    f.write("\t},\n")

f.write('};\n\n')
f.flush()
f.close()
