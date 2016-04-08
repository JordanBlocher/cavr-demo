using RAIN.Motion;
using RAIN.Serialization;

namespace Assets.AI.CustomMotors
{
    [RAINSerializableClass]
    public class OverridableMotor : BasicMotor
    {
        public bool OverrideMotor { get; set; }

        public override void UpdateMotionTransforms()
        {
            if (OverrideMotor)
            {
                AI.Kinematic.Position = AI.Body.GetComponent<UnityEngine.Rigidbody>().position;
                AI.Kinematic.Orientation = AI.Body.GetComponent<UnityEngine.Rigidbody>().rotation.eulerAngles;
                AI.Kinematic.Velocity = AI.Body.GetComponent<UnityEngine.Rigidbody>().velocity;
                AI.Kinematic.Acceleration = AI.Body.GetComponent<UnityEngine.Rigidbody>().velocity;
                AI.Kinematic.AngularAcceleration = AI.Body.GetComponent<UnityEngine.Rigidbody>().angularVelocity;
            }
            else
            {
                base.UpdateMotionTransforms();
            }
        }

        public override void ApplyMotionTransforms()
        {
            if (!OverrideMotor)
                base.ApplyMotionTransforms();
        }
    }
}
