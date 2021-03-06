/* This file is part of Into.
 * Copyright (C) Intopii 2013.
 * All rights reserved.
 *
 * Licensees holding a commercial Into license may use this file in
 * accordance with the commercial license agreement. Please see
 * LICENSE.commercial for commercial licensing terms.
 *
 * Alternatively, this file may be used under the terms of the GNU
 * Affero General Public License version 3 as published by the Free
 * Software Foundation. In addition, Intopii gives you special rights
 * to use Into as a part of open source software projects. Please
 * refer to LICENSE.AGPL3 for details.
 */

#ifndef _PIICOORDINATETRACKERNODE_H
#define _PIICOORDINATETRACKERNODE_H

#include "PiiTrackerTrajectoryNode.h"
#include <PiiVector.h>


/**
 * A trajectory node for PiiCoordinateTracker. Uses [PiiVector]
 * "PiiVectors" as measurements.
 *
 */
template <class T, int D> class PiiCoordinateTrackerNode : public PiiTrackerTrajectoryNode<PiiVector<T,D>, PiiCoordinateTrackerNode<T,D> >
{
public:
  typedef PiiVector<T,D> MeasurementType;
  typedef PiiCoordinateTrackerNode<T,D> NodeType;
  typedef PiiTrackerTrajectoryNode<MeasurementType, NodeType> ParentType;

  /**
   * Create a new trajectory node with the given measurement, time
   * instant and next node in chain.
   *
   * @param measurement the measurement
   *
   * @param t current time instant
   *
   * @param fitness the "fitness score" of the measurement, compared
   * to the *previous* prediction.
   *
   * @param next the next node in chain
   */
  PiiCoordinateTrackerNode(const MeasurementType& measurement = MeasurementType(), int t = 0, double fitness = 0, NodeType* next = 0) :
    ParentType(measurement, t, next),
    _dMeasurementFitness(fitness), _dTrajectoryFitness(0),
    _pPrediction(0)
  {}

  /**
   * Copy constructor.
   */
  PiiCoordinateTrackerNode(const PiiCoordinateTrackerNode& other) :
    ParentType(other),
    _dMeasurementFitness(other._dMeasurementFitness),
    _dTrajectoryFitness(other._dTrajectoryFitness),
    _pPrediction(other._pPrediction ? new MeasurementType(*other._pPrediction) : 0)
  {}

  /**
   * Create a new node with all values initialized to zeros.
   */
  PiiCoordinateTrackerNode(NodeType* next) :
    ParentType(MeasurementType(), 0, next),
    _dMeasurementFitness(0), _dTrajectoryFitness(0), _pPrediction(0)
  {}

  /**
   * Deletes the prediction.
   */
  ~PiiCoordinateTrackerNode() { delete _pPrediction; }

  /**
   * Get a pointer to the predicted next point in trajectory.
   *
   * @return the prediction or 0 if no prediction has been stored
   */
  MeasurementType* prediction() const { return _pPrediction; }
  /**
   * Set the prediction. The node takes the ownership of
   * `prediction`.
   */
  void setPrediction(MeasurementType* prediction) { _pPrediction = prediction; }

  /**
   * Set the fitness of the measurement stored in this trajectory
   * node.
   */
  void setMeasurementFitness(double fitness) { _dMeasurementFitness = fitness; }
  /**
   * Get the fitness of the measurement stored in this trajectory
   * node.
   */
  double measurementFitness() const { return _dMeasurementFitness; }

  /**
   * Set the fitness of the whole trajectory ending at this node.
   */
  void setTrajectoryFitness(double fitness) { _dTrajectoryFitness = fitness; }
  /**
   * Get the fitness of the whole trajectory ending at this node.
   */
  double trajectoryFitness() const { return _dTrajectoryFitness; }

  struct LessThan;
  struct GreaterThan;

private:
  /**
   * The fitness of the measurement, compared to the previous
   * prediction.
   */
  double _dMeasurementFitness;

  /**
   * The fitness of the whole trajectory up to this point.
   */
  double _dTrajectoryFitness;

  /**
   * Prediction for the next measurement point.
   */
  MeasurementType* _pPrediction;
};


/**
 * A functor for sorting nodes in descending order based on the
 * trajectory's fitness.
 */
template <class T, int D> struct PiiCoordinateTrackerNode<T,D>::GreaterThan
{
  typedef PiiCoordinateTrackerNode<T,D> NodeType;
  // Compare trajectories based on their fitness.
  bool operator()(NodeType* n1, NodeType* n2) const
  {
    return n1->trajectoryFitness() > n2->trajectoryFitness();
  }
};

/**
 * A functor for sorting nodes in ascending order based on the
 * trajectory's fitness.
 */
template <class T, int D> struct PiiCoordinateTrackerNode<T,D>::LessThan
{
  typedef PiiCoordinateTrackerNode<T,D> NodeType;
  // Compare trajectories based on their fitness.
  bool operator()(NodeType* n1, NodeType* n2) const
  {
    return n1->trajectoryFitness() < n2->trajectoryFitness();
  }
};

#endif //_PIICOORDINATETRACKERNODE_H
