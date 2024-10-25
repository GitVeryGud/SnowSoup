# SnowSoup

## SnowSoup Setup

In order to start using SnowSoup, the first thing you need to do is download the framework file from the GitHub repository.

You then need to put the framework file in your project folder.

Next thing, go to build settings in your project and add the path to the headers in ‘Header Search Path’ and the path to the framework in ‘Framework Search Path’. In build phases, add the framework file to ‘Link Binary with Libraries’.

You are now ready to start SnowSouping.

## Basic Project

The most basic project using SnowSoup capabilities is:

… CODE …

You need to create an instance of the SnowSoup class. This class handle’s everything that SnowSoup can offer: rendering, input, collision and so on.

SnowSoup uses a node system to manage the objects in your game. All you need to do to create your own game is to create these nodes with your own code and add them to the SnowSoup instance.

The engine already comes with a few nodes of which you can create subclasses. This is the way you create the nodes for your game, you must subclass the already existing node classes and add code to them. Here’s an example:

… CODE …

You can also use instances of the already existing nodes, but you can’t add code of your own to them.

To add the nodes to SnowSoup, all you need to do is call the method:

	SnowSoupInstance.addNode(*pointer to your node here*);

And after adding all of your nodes to the engine, to run the game you call:
	
	SnowSoup.run();

And that’s all!

## Classes

In this segment, all of the classes that make SnowSoup are explained in great detail in order to make you understand everything one must know to develop a game using this engine.

### Node

The Node class is the most basic node type in SnowSoup. It’s only purpose is to exist inside the SceneGraph of the engine and to have virtual methods that must be implemented by its subclasses. All other nodes are subclasses of this node and implement the Update virtual method.

### Transform

This class has the purpose of giving nodes a representation in 3D space. This means it gives a node position, rotation and scale properties. Every node that can move in 3D space is a subclass of this class.

### Model

The Model class stores a 3D model of your choosing. The best 3D format for SnowSoup is COLLADA, also known as .dae, so make sure to use it. This class inherits from Transform.

### Collider

The Collider class represents an axis-aligned bounding box that detects collision with other colliders. This class inherits from Transform.

### Projection Camera

The projection camera is the node from which the player can see the 3D world. Inherits from Transform.