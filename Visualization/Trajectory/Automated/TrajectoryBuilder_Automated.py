import matplotlib.pyplot as pyplot
from dataclasses import dataclass

@dataclass
class ParticleData:
    x: float
    y: float
    
@dataclass
class ParticleGridData:
    enabled : bool = False
    resolutionX: int = 0
    resolutionY: int = 0
    deltaX: float = 0
    deltaY: float = 0
    originX: float = 0
    originY: float = 0
    

# Main data
particleSamples : dict[list[ParticleData]] = dict()

# Additional data
particleGridData = ParticleGridData()

# Data parsing

#filePath = input("File Path: ")
filePath = "./particle_trajectories_auto_vis.txt"
lines = open(filePath, "r").readlines()

for line in lines:
    
    line = line.strip()
    
    # Parsing particle grid parameters
    
    if line.startswith("Particle Grid Parameters:"):
        particleGridData.enabled = True
    
    elif line.startswith("Resolution X:"):
        line = line.replace("Resolution X: ", "")
        particleGridData.resolutionX = int(line)
    
    elif line.startswith("Resolution Y:"):
        line = line.replace("Resolution Y: ", "")
        particleGridData.resolutionY = int(line)
        
    elif line.startswith("Delta X:"):
        line = line.replace("Delta X: ", "")
        particleGridData.deltaX = float(line)
        
    elif line.startswith("Delta Y:"):
        line = line.replace("Delta Y: ", "")
        particleGridData.deltaY = float(line)
        
    elif line.startswith("Origin: "):
        line = line.replace("Origin: ", "")
        parts = line.split(",")
        particleGridData.originX = float(parts[0])
        particleGridData.originY = float(parts[1])
        
    
    # Parsing main data
    
    # Iteration line (skipped)
    elif line.startswith("Iteration:"):
        continue
    
    # Parsing particle data
    elif " | " in line:
        parts = line.split(" | ")
        
        particleID = int(parts[0])
        line = parts[1]
        
        if line.startswith("Location: "):
            
            # Ensure dict contains data for this particle ID
            if not particleID in particleSamples:
                particleSamples[particleID] = []
            
            line = line.replace("Location: ", "")
            parts = line.split(",")
            x = float(parts[0])
            y = float(parts[1])
            
            particleSamples[particleID].append(ParticleData(x, y))
            
        
# for particleID in range(len(particleSamples)):
#     print(f"Particle {particleID}:")
#     for sample in particleSamples[particleID]:
#         print(f"({sample.x}, {sample.y})")
        
#     print("\n")


# Plotting particle grid
if particleGridData.enabled:
    
    gridXValues = [
        particleGridData.originX + i * particleGridData.deltaX
        for i in range(particleGridData.resolutionX)
    ]
    
    gridYValues = [
        particleGridData.originY + j * particleGridData.deltaY
        for j in range(particleGridData.resolutionY)
    ]
    
    for x in gridXValues:
        pyplot.axvline(x=x, color='lightgray', linestyle='--', linewidth=0.5)
    
    for y in gridYValues:
        pyplot.axhline(y=y, color='lightgray', linestyle='--', linewidth=0.5)

# Plotting the trajectories
for particleID in particleSamples:
    
    xValues = [sample.x for sample in particleSamples[particleID]]
    yValues = [sample.y for sample in particleSamples[particleID]]
    
    pyplot.plot(xValues, yValues, label=f"Particle {particleID}", linewidth=1)
    pyplot.scatter(xValues, yValues, s=2)
    
    for i in range(0, len(xValues), len(xValues) // 5):
        pyplot.annotate('', xy=(xValues[i + 1], yValues[i + 1]), xytext=(xValues[i], yValues[i]),
            arrowprops=dict(color='grey', lw=0.5))


pyplot.title("Particle Trajectories")
pyplot.xlabel("X Position")
pyplot.ylabel("Y Position")
pyplot.legend()
pyplot.show()