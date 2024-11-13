import express from 'express';
import db from '../db/conn.mjs';
import { ObjectId } from 'mongodb';

const router = express.Router();

router.get('/', async (_req, res) => {
  const tempReadings = await db.collection('temp_readings').find({}).toArray();
  res.send(tempReadings).status(200);
});

export default router;