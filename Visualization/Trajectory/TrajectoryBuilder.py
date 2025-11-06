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
particleSamples : list[list[ParticleData]] = []

# Additional data
particleGridData = ParticleGridData()

# Data parsing

filePath = input("File Path: ")
lines = open(filePath, "r").readlines()

currentParticleID = 0
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
    
    elif line.startswith("Iteration:"):
        currentParticleID = 0
        
    elif line.startswith("Location: "):
        
        if len(particleSamples) <= currentParticleID:
            particleSamples.append([])
        
        line = line.replace("Location: ", "")
        parts = line.split(",")
        x = float(parts[0])
        y = float(parts[1])
        
        particleSamples[currentParticleID].append(ParticleData(x, y))
        
        currentParticleID += 1
        
# for particleID in range(len(particleSamples)):
#     print(f"Particle {particleID}:")
#     for sample in particleSamples[particleID]:
#         print(f"({sample.x}, {sample.y})")
        
#     print("\n")


# Plotting particle grid
if particleGridData.enabled:
    
    gridXValues = [
        particleGridData.originX + i * particleGridData.deltaX
        for i in range(particleGridData.resolutionX + 1)
    ]
    
    gridYValues = [
        particleGridData.originY + j * particleGridData.deltaY
        for j in range(particleGridData.resolutionY + 1)
    ]
    
    for x in gridXValues:
        pyplot.axvline(x=x, color='lightgray', linestyle='--', linewidth=0.5)
    
    for y in gridYValues:
        pyplot.axhline(y=y, color='lightgray', linestyle='--', linewidth=0.5)

# Plotting the trajectories
for particleID in range(len(particleSamples)):
    
    xValues = [sample.x for sample in particleSamples[particleID]]
    yValues = [sample.y for sample in particleSamples[particleID]]
    
    pyplot.plot(xValues, yValues, label=f"Particle {particleID}")
    pyplot.scatter(xValues, yValues)


pyplot.title("Particle Trajectories")
pyplot.xlabel("X Position")
pyplot.ylabel("Y Position")
pyplot.legend()
pyplot.show()
input("Press Enter to Continue...")