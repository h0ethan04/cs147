
import express from "express";
import cors from "cors";
import "./loadenv.mjs";
import "express-async-errors";
import tempReadings from "./routes/tempReadings.mjs";
import serverless from "serverless-http";

const PORT = process.env.PORT || 3000;
const app = express();

app.use(cors());
app.use(express.json());

app.use("/temp-readings", tempReadings);

app.use((err, _req, res, next) => {
	res.status(500).send("An unexpected server error occurred.");
})

// app.listen(PORT, () => {
// 	console.log(`Server listening on port: ${PORT}`);
// });

export const handler = serverless(app);