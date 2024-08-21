
import asyncio
import websockets

async def echo(websocket, path):
        async for message in websocket:
                print(f"Receive from client : {message}")
                await websocket.send(f"Server recieved : {message}")

async def main():
        server = await websockets.serve(echo,"0.0.0.0",8765)
        print("Server Start,waiting for communication...")
        await server.wait_closed()

if __name__== "__main__":
        asyncio.run(main())
