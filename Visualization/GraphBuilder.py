import matplotlib.pyplot as pyplot

print("File Name: ")
file_path = input()
print("Index of value set to display: ")
value_set_id = int(input())

keys = []
values_matr : list[list] = []

# Parsing file
file = open(file_path)

for line in file.readlines():
    segments = line.split(", ")
    
    if len(segments) > 0:
        keys.append(float(segments[0]))
        
        valuesSegments = segments[1::]
        
        for i in range(len(valuesSegments)):
            
            if len(values_matr) <= i:
                values_matr.append(list())
                
            values_matr[i].append((float)(valuesSegments[i].replace('\n', '')))
            
            
# Create the plot
pyplot.plot(keys, values_matr[value_set_id])


# Reversing x-axis
axis = pyplot.gca()

axis.invert_xaxis()

# Add labels and a title
# pyplot.xlabel('X-axis Label')
# pyplot.ylabel('Y-axis Label')
# pyplot.title('My First Line Graph')

# Display the plot
pyplot.show()