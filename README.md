myLocator

# run this code in maya script editor to see node in action!
import maya.cmds as cmds
node = cmds.createNode('myLocator')
transform = cmds.listRelatives(node, parent=True, path=True)[0]
locator = cmds.spaceLocator()[0]

cmds.connectAttr('%s.outPoint' % node, '%s.translate'%locator)
cmds.connectAttr('%s.worldMatrix[0]'%transform, '%s.inMatrix'%node)