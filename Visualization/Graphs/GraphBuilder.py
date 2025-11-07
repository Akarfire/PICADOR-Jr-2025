import matplotlib.pyplot as pyplot

print("File Path: ")
file_path = input()

# Actual Data
keys = []
values_matr : list[list] = []

# Meta Data
keys_label = ""
values_labels : list[str] = []

# Tags
inverse_x = False
inverse_y = False

# Parsing file
file = open(file_path)

for line in file.readlines():
    
    # Skipping empty lines
    if line.count(' ') == len(line):
        continue
    
    # Parsing tags
    if line.startswith("/"):
        if line == "/InverseX\n": inverse_x = True
        elif line == "/InverseY\n": inverse_y = True
    
    # Parsing meta data
    elif line.startswith("#"):
        segments = line.split(",")
        
        if len(segments) > 0:
            keys_label = segments[0].replace('#', '')
            values_labels = [i.replace('\n', '') for i in segments[1::]]
            
    # Parsing actual data
    else:
        segments = line.split(",")
        
        if len(segments) > 0:
            keys.append(float(segments[0].replace('\n', '')))
            
            valuesSegments = segments[1::]
            
            for i in range(len(valuesSegments)):
                
                if len(values_matr) <= i:
                    values_matr.append(list())
                    
                values_matr[i].append((float)(valuesSegments[i].replace('\n', '')))
            
file.close()

for i in range(len(values_matr)):
    
    pyplot.figure()
    
    # Creating the plot
    pyplot.plot(keys, values_matr[i], label=values_labels[i])

    # Inversing axes with tags

    if inverse_x:
        axis = pyplot.gca()
        axis.invert_xaxis()
        
    if inverse_y:
        axis = pyplot.gca()
        axis.invert_yaxis()
        
    # Add label

    pyplot.xlabel(keys_label)

    # Displaying the plot

    pyplot.legend()
    pyplot.show(block=False)


input("'Enter' to exit")

    