import express, {Request, Response} from 'express';
import { setAValue, setBValue } from './ser';


const app = express()

app.get("/", (req: Request, res: Response) => {
    res.send("your mom sucks")
})

app.get("/a", (req: Request, res: Response) => {
    if (req.query["value"] != undefined) setAValue(req.query["value"]!.toString())
    res.send(req.query["value"])
})

app.get("/b", (req: Request, res: Response) => {
    if (req.query["value"] != undefined) setBValue(req.query["value"]!.toString())
    res.send(req.query["value"])
})




export default app