
import asyncio
import websockets

connected_clients = set()

async def handle_client(websocket, path):
        connected_clients.add(websocket)
        try:
                async for message in websocket:
                        print(f"Receive message : {message}")
                        await asyncio.gather(*[client.send(message) for client in connected_clients])
        finally:
                connected_clients.remove(websocket)

async def main():
        server = await websockets.serve(handle_client,"0.0.0.0",8765)
        print("Server Start,waiting for communication...")
        await server.wait_closed()

if __name__== "__main__":
        asyncio.run(main())
