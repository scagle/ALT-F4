import matplotlib.pyplot as plt
import numpy as np

keys = []
values = dict()

with open("attributes.csv", "r") as f:
    lines = f.readlines()

    # initialize the dictionary
    for key in lines[0].strip().split(","):
        print(key, end=" ")
        values[key] = [];
        keys.append(key)
    print("")

    # fill dictionary
    for line in lines[1:]:
        line = line.strip()
        for i, value in enumerate( line.split( "," ) ):
            values[keys[i]].append( float( value ) )
            print(value, end=" ")
        print("")

fig = plt.figure()
ax = plt.axes()

for key in keys:
    maximum_value = max(values[key])
    if ( maximum_value != 0 ):
        normalized_data = [ float( v / maximum_value ) for v in values[key] ]
        ax.plot(np.arange( 0, len( normalized_data ) ), normalized_data, label=key )
    else:
        ax.plot(np.arange( 0, len( values[key] ) ), values[key], label=key )

plt.legend()
plt.show()
