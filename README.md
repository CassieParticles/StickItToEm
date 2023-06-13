# StickItToEm
A remake of the game I made for Uni coursework, made using OpenGL

TODO:
	TerrainManager:
		Step1:
			Take in size of arena (number of squares (x,y))
				Create array for scalar data, lines for collision, and buffers for rendering terrain triangles/lines
			Take in scalar data to generate arena
				Take in pointer to array, copy data to array in terrainManager
			Generate terrain using marching square algorithm
				Create function to generate marchingSquares
				Generate triangles and lines
					Fill buffers for rendering triangles/lines

			Render triangles and lines to screen
				Marching squares takes up full screen irregardless of screen size
					Use screen size and grid size to transform vertex positions
	Player:

	Collision:

	Weapons:

