import express from 'express';
import db from '../db/conn.mjs';
import { ObjectId } from 'mongodb';

const router = express.Router();

router.get('/', async (_req, res) => {
  const tempReadings = await db.collection('temp_readings').find({}).toArray();
  res.send(tempReadings).status(200);
});

router.post('/', async (req, res) => {
  const {tempReading, gasReading} = req.body;
  await db.collection('temp_readings').insertOne({tempReading: tempReading, gasReading: gasReading, date: new Date()});
  res.send(tempReading).status(201);
});

export default router;