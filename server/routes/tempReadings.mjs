import express from 'express';
import db from '../db/conn.mjs';
import { ObjectId } from 'mongodb';

const router = express.Router();

router.get('/', async (_req, res) => {
  const tempReadings = await db.collection('temp_readings').find({}).toArray();
  res.status(200).send(tempReadings);
});

router.post('/', async (req, res) => {
  const {tempReading, gasReading} = req.body;
  await db.collection('temp_readings').insertOne({tempReading: tempReading, gasReading: gasReading, date: new Date()});
  res.status(201).send({tempReading, gasReading});
});

export default router;